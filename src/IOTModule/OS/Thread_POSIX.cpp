//
// Thread_POSIX.cpp
//


#include "Thread_POSIX.h"
#include "../Exception.h"
#include "../ErrorHandler.h"
//#include "../Logger.h"
//#include "../ThreadManager.h"
#include <signal.h>

#if !defined(CORE_OS_FAMILY_WINDOWS)
#include <syscall.h>
#include <sys/prctl.h>
#endif

#define TIDBUFNUM 18
#define PIDBUFNUM 19

//
// Block SIGPIPE in main thread.
//
#if defined(CORE_OS_FAMILY_UNIX)
namespace
{
	class SignalBlocker
	{
	public:
		SignalBlocker()
		{
			sigset_t sset;
			sigemptyset(&sset);
			sigaddset(&sset, SIGPIPE); 
			pthread_sigmask(SIG_BLOCK, &sset, 0);
		}
		~SignalBlocker()
		{
		}
	};
	
	static SignalBlocker signalBlocker;
}
#endif


namespace CORE {


pthread_key_t ThreadImpl::m_currentKey;
bool ThreadImpl::m_haveCurrentKey = false;


ThreadImpl::ThreadImpl():
	m_pData(new ThreadData),	
	m_tid(0)
{
	m_pData->pTarget = NULL;
	m_pData->running = 0;
	
	if (!m_haveCurrentKey)
	{
		if (pthread_key_create(&m_currentKey, NULL))
		{
		//	_LogError<<"cannot allocate thread context key"<<_LogErrorEnd;
			throw SystemException("cannot allocate thread context key");
		}
		m_haveCurrentKey = true;
	}
}

			
ThreadImpl::~ThreadImpl()
{
	if (m_pData->pTarget)
	{
		pthread_detach(m_pData->thread);
	}
}


void ThreadImpl::setPriorityImpl(Int32 prio)
{
	if (prio != m_pData->prio)
	{
		m_pData->prio = prio;
		if (m_pData->pTarget)
		{
			struct sched_param par;
			par.sched_priority = mapPrio(m_pData->prio);
//			if (pthread_setschedparam(m_pData->thread, SCHED_OTHER, &par))
			if (pthread_setschedparam(m_pData->thread, SCHED_RR, &par))
			{
	//			_LogError<<"cannot set thread priority"<<_LogErrorEnd;
				throw SystemException("cannot set thread priority");
			}
		}
	}
}


void ThreadImpl::stopImpl()
{
	if (m_pData->pTarget)
	{
		pthread_detach(m_pData->thread);
		m_pData->pTarget=0;
	}
}

void ThreadImpl::startImpl(Runnable& target)
{
	printf("--->ThreadImpl::startImpl 00 \n");

	if (m_pData->pTarget) 
	{
//		_LogError<<"thread already running"<<_LogErrorEnd;
		printf("--->ThreadImpl::startImpl() thread already running \n");

		throw SystemException("thread already running");
	}

	m_pData->pTarget = &target;
	m_pData->running = 1;

	int ret = 0,stacksize = (4*1024*1024); /*thread 堆栈设置为4M，stacksize以字节为单位。*/

	pthread_attr_t attr;
	ret = pthread_attr_init(&attr); /*初始化线程属性*/
	if (ret != 0)
	{
	//	_LogError<<"cannot pthread_attr_init, errno:"<<strerror(errno)<<_LogErrorEnd;
		printf("--->ThreadImpl::startImpl() cannot pthread_attr_init, errno:%s\n",strerror(errno));
		throw SystemException("cannot pthread_attr_init");
	}
/*
	ret = pthread_attr_setstacksize(&attr, stacksize);
	if(ret != 0)
	{
	//	_LogError<<"cannot pthread_attr_setstacksize, errno:"<<strerror(errno)<<_LogErrorEnd;
		printf("--->ThreadImpl::startImpl() cannot pthread_attr_setstacksize, errno:%s\n",strerror(errno));
		throw SystemException("cannot pthread_attr_setstacksize");
	}
*/
	printf("--->ThreadImpl::startImpl 11 \n");

	if (pthread_create(&m_pData->thread, &attr, entry, this))
	{
		m_pData->pTarget = 0;
		m_pData->running = 0;
		m_pData->done.set();
//		_LogError<<"cannot start thread, errno:"<<strerror(errno)<<_LogErrorEnd;
		printf("--->ThreadImpl::startImpl() cannot start thread, errno:%s\n",strerror(errno));
		throw SystemException("cannot start thread");
	}

	if (m_pData->prio != PRIO_NORMAL_IMPL)
	{
		struct sched_param par;
		par.sched_priority = mapPrio(m_pData->prio);
//		if (pthread_setschedparam(m_pData->thread, SCHED_OTHER, &par))
		if (pthread_setschedparam(m_pData->thread, SCHED_RR, &par))
		{
	//		_LogError<<"cannot set thread priority"<<_LogErrorEnd;
			printf("--->ThreadImpl::startImpl() cannot set thread priority, errno:%s\n",strerror(errno));
			throw SystemException("cannot set thread priority");
		}
	}
	printf("--->ThreadImpl::startImpl() success\n");

//	_LogDebug<<"thread  startImpl() success! m_tid = "<<m_tid<<_LogDebugEnd;
}


void ThreadImpl::joinImpl()
{
	if(m_pData->pTarget == 0)
	{
		if (m_pData->running == 0)
		{
			return;
		}
		else
		{//entry有可能没有,需要等
		}	
	}
	


	m_pData->done.wait();
	void* result;
	if (pthread_join(m_pData->thread, &result))
	{
	//	_LogError<<"cannot join thread ()"<<_LogErrorEnd;
		throw SystemException("cannot join thread"); 
	}

	m_pData->pTarget = 0;
}


bool ThreadImpl::joinImpl(Int32 milliseconds)
{
	if(m_pData->pTarget == 0)
	{		
		if (m_pData->running == 0)
		{
			return true;
		}
		else
		{//entry有可能没有,需要等
		}			
	}

	if (m_pData->done.tryWait(milliseconds))
	{
		void* result;
		if (pthread_join(m_pData->thread, &result))
		{
//			_LogError<<"cannot join thread (Int32)"<<_LogErrorEnd;
			throw SystemException("cannot join thread");
		}

		m_pData->pTarget = 0;
		return true;
	}
	else 
	{
		return false;
	}
}


bool ThreadImpl::isRunningImpl() const
{
	return m_pData->running != 0;
}


ThreadImpl* ThreadImpl::currentImpl()
{
	if (m_haveCurrentKey)
	{
		return reinterpret_cast<ThreadImpl*>(pthread_getspecific(m_currentKey));
	}
	else
	{
		return 0;
	}
}


void* ThreadImpl::entry(void* pThread)
{
	pthread_setspecific(m_currentKey, pThread);

#if defined(CORE_OS_FAMILY_UNIX)
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

	ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
	AutoPtr<ThreadData> pData = pThreadImpl->m_pData;
/*
	//2011-1-14 guec add 必须在这里生成m_tid
	UInt32* pTid = (UInt32*) pData->thread;
	UInt32 tid = pTid[TIDBUFNUM];
*/
#ifndef WIN32
	UInt32 tid = syscall(__NR_gettid); //gettid();
#else
	UInt32 tid = GetCurrentThreadId();
#endif

	pThreadImpl->SetTid(tid);
//	_LogDebug<<"ThreadImpl::entry() tid = "<<tid<<_LogDebugEnd;
	

	CData name = typeid(*(pData->pTarget)).name();
//	DefaultThreadManager().AddThreadtoList(tid, name);//添加到线程管理列表

	try
	{
		pData->pTarget->run();
	}
	catch (...)
	{
		Thread* thread;
		thread=Thread::current();
	//	_LogFatal<<"thread fatal error--"<<thread->getName().c_str()<<_LogFatalEnd;

		pData->done.set();
		pData->running = 0;
//		core_throw_null(); //
	}
	
//	DefaultThreadManager().DelThreadfromList(tid);//从线程管理列表中删除
	
//	pData->running = 0;
//	pData->pTarget = 0;//必须注释掉20100227-guec
	pData->done.set();
	pData->running = 0;
	return 0;
}


Int32 ThreadImpl::mapPrio(Int32 prio)
{
//	static const Int32 pmin = sched_get_priority_min(SCHED_OTHER);
//	static const Int32 pmax = sched_get_priority_max(SCHED_OTHER);
	static const Int32 pmin = sched_get_priority_min(SCHED_RR);
	static const Int32 pmax = sched_get_priority_max(SCHED_RR);

	switch (prio)
	{
		case PRIO_LOWEST_IMPL:
		{
			return pmin;
		}	
		case PRIO_LOW_IMPL:
		{
			return pmin + (pmax - pmin)/4;
		}	
		case PRIO_NORMAL_IMPL:
		{
			return pmin + (pmax - pmin)/2;
		}	
		case PRIO_HIGH_IMPL:
		{
			return pmin + 3*(pmax - pmin)/4;
		}
		case PRIO_HIGHEST_IMPL:
		{
			return pmax;
		}	
		default:
		{
			return -1;
		}
	}
	return -1; 
}


}

