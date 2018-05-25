//
// Thread_WIN32.h
//


#include "Thread_WIN32.h"
#include "../Exception.h"
#include "../ErrorHandler.h"
#include <process.h>
#include "../Thread.h"


namespace CORE {


DWORD ThreadImpl::m_currentKey = TLS_OUT_OF_INDEXES;


ThreadImpl::ThreadImpl(): m_pTarget(0), m_thread(0), m_nPrio(PRIO_NORMAL_IMPL)
{
	if (m_currentKey == TLS_OUT_OF_INDEXES)
	{
		m_currentKey = TlsAlloc();
		if (m_currentKey == TLS_OUT_OF_INDEXES)
		{
			throw SystemException("cannot allocate thread context key");
		}
	}
}

			
ThreadImpl::~ThreadImpl()
{
	if (m_thread) 
	{
		CloseHandle(m_thread);
	}
}


void ThreadImpl::setPriorityImpl(Int32 prio)
{
	if (prio != m_nPrio)
	{
		m_nPrio = prio;
		if (m_thread)
		{
			if (SetThreadPriority(m_thread, m_nPrio) == 0)
			{
				throw SystemException("cannot set thread priority");
			}
		}
	}
}


void ThreadImpl::stopImpl()
{
	if (m_thread) 
	{
		CloseHandle(m_thread);
		m_thread=0;
	}
}


void ThreadImpl::startImpl(Runnable& target)
{
	if (m_thread) 
	{
		throw SystemException("thread already running");
	}

	m_pTarget = &target;
#if defined(_DLL)
	DWORD threadId;
	m_thread = CreateThread(NULL, 0, entry, this, 0, &threadId);
#else
	unsigned threadId;
	m_thread = (HANDLE) _beginthreadex(NULL, 0, entry, this, 0, &threadId);
#endif
	if (!m_thread)
	{
		throw SystemException("cannot create thread");
	}
	if (m_nPrio != PRIO_NORMAL_IMPL && !SetThreadPriority(m_thread, m_nPrio))
	{
		throw SystemException("cannot set thread priority");
	}
}


void ThreadImpl::joinImpl()
{
	if (!m_thread) return;

	switch (WaitForSingleObject(m_thread, INFINITE))
	{
		case WAIT_OBJECT_0:
		{
			m_thread=0;
			return;
		}
		default:
		{
			throw SystemException("cannot join thread");
		}
	}
}


bool ThreadImpl::joinImpl(Int32 milliseconds)
{
	if (!m_thread) return true;

	switch (WaitForSingleObject(m_thread, milliseconds + 1))
	{
		case WAIT_TIMEOUT:
		{
			return false;
		}
		case WAIT_OBJECT_0:
		{
			m_thread=0;
			return true;
		}
		default:
		{
			throw SystemException("cannot join thread");
		}
	}
}


bool ThreadImpl::isRunningImpl() const
{
	if (m_thread)
	{
		DWORD ec = 0;
		return GetExitCodeThread(m_thread, &ec) && ec == STILL_ACTIVE;
	}
	return false;
}


ThreadImpl* ThreadImpl::currentImpl()
{
	if (m_currentKey == TLS_OUT_OF_INDEXES)
	{
		return 0;
	}
	else
	{
		return (ThreadImpl*) TlsGetValue(m_currentKey);
	}
}


#if defined(_DLL)
DWORD WINAPI ThreadImpl::entry(LPVOID pThread)
#else
unsigned __stdcall ThreadImpl::entry(void* pThread)
#endif
{
	TlsSetValue(m_currentKey, pThread);
	try
	{
		reinterpret_cast<ThreadImpl*>(pThread)->m_pTarget->run();
	}
	catch (...)
	{
		Thread* thread;
		thread=Thread::current();
	//	_LogFatal<<"thread fatal error--"<<thread->getName().c_str()<<_LogFatalEnd;
//		core_throw_null();
	}
	
	return 0;
}


}

