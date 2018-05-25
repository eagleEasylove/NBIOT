//
// Event_POSIX.cpp
//


#include "Event_POSIX.h"
#include <sys/time.h>


namespace CORE {


EventImpl::EventImpl(bool autoReset): m_auto(autoReset), m_state(false)
{
	if (pthread_mutex_init(&m_mutex, NULL))
	{
		throw SystemException("cannot create event (mutex)");
	}
	if (pthread_cond_init(&m_cond, NULL))
	{
		throw SystemException("cannot create event (condition)");
	}
}


EventImpl::~EventImpl()
{
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
}


void EventImpl::waitImpl()
{
	if (pthread_mutex_lock(&m_mutex))
	{
		throw SystemException("wait for event failed (lock)"); 
	}
	while (!m_state) 
	{
		if (pthread_cond_wait(&m_cond, &m_mutex))
		{
			pthread_mutex_unlock(&m_mutex);
			throw SystemException("wait for event failed");
		}
	}
	if (m_auto)
	{
		m_state = false;
	}
	pthread_mutex_unlock(&m_mutex);
}


bool EventImpl::waitImpl(Int32 milliseconds)
{
	Int32 rc = 0;
	struct timespec abstime;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
	abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		throw SystemException("wait for event failed (lock)"); 
	}
	
	while (!m_state) 
	{
		if ((rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
		{
			if (rc == ETIMEDOUT) 
			{
				break;
			}
			pthread_mutex_unlock(&m_mutex);
			throw SystemException("cannot wait for event");
		}
	}
	
	if (rc == 0 && m_auto)
	{
		m_state = false;
	}
	pthread_mutex_unlock(&m_mutex);
	return rc == 0;
}


}

