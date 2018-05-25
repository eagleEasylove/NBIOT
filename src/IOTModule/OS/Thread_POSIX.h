//
// Thread_POSIX.h
//


#ifndef Thread_POSIX_H
#define Thread_POSIX_H


#include "../common_config_base.h"
#include "../Runnable.h"
#include "../Event.h"
#include "../RefCountedObject.h"
#include "../AutoPtr.h"
#include <pthread.h>
#if !defined(CORE_NO_SYS_SELECT_H)
#include <sys/select.h>
#endif
#include <errno.h>


namespace CORE {


class CORE_API ThreadImpl
{
public:
	enum Priority
	{
		PRIO_LOWEST_IMPL,
		PRIO_LOW_IMPL,
		PRIO_NORMAL_IMPL,
		PRIO_HIGH_IMPL,
		PRIO_HIGHEST_IMPL
	};

	ThreadImpl();
	
	~ThreadImpl();

	Runnable& targetImpl() const;
	
	void setPriorityImpl(Int32 prio);
	
	Int32 getPriorityImpl() const;

	UInt32 getTidImpl()const;
	
	void startImpl(Runnable& target);

	void stopImpl();

	void joinImpl();
	
	bool joinImpl(Int32 milliseconds);
	
	bool isRunningImpl() const;
	
	static void sleepImpl(Int32 milliseconds);
	
	static void yieldImpl();
	
	static ThreadImpl* currentImpl();
	void SetTid(UInt32 tid){m_tid=tid;};//guec 2011-1-14
protected:
	static void* entry(void* pThread);
	
	static Int32 mapPrio(Int32 prio);

private:
	struct ThreadData: public RefCountedObject
	{
		ThreadData():
			pTarget(0),
			thread(0),
			prio(PRIO_NORMAL_IMPL),
			done(false),
			running(0)
		{
		}

		Runnable* pTarget;
		pthread_t thread;
		Int32     prio;
		Event     done;
		Int32	running;
	};
	
	AutoPtr<ThreadData> m_pData;
	
	static pthread_key_t m_currentKey;
	static bool          m_haveCurrentKey;
	UInt32 m_tid;
};


//
// inlines
//
inline Int32 ThreadImpl::getPriorityImpl() const
{
	return m_pData->prio;
}


inline void ThreadImpl::sleepImpl(Int32 milliseconds)
{
	struct timeval tv;
	tv.tv_sec  = milliseconds / 1000;
	tv.tv_usec = (milliseconds % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv); 	
}


inline void ThreadImpl::yieldImpl()
{
	sched_yield();
}


inline UInt32 ThreadImpl::getTidImpl() const
{
	return m_tid;
}

} 


#endif 

