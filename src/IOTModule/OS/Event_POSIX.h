//
// Event_POSIX.h
//


#ifndef Event_POSIX_H
#define Event_POSIX_H


#include "../common_config_base.h"
#include "../Exception.h"
//#include <Thread.h>
#include <pthread.h>
#include <errno.h>


namespace CORE {


class CORE_API EventImpl
{
protected:
	EventImpl(bool autoReset);
	
	~EventImpl();
	
	void setImpl();
	
	void waitImpl();
	
	bool waitImpl(Int32 milliseconds);
	
	void resetImpl();
	
private:
	bool            m_auto;
	volatile bool   m_state;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
};


//
// inlines
//
inline void EventImpl::setImpl()
{
	if (pthread_mutex_lock(&m_mutex))
	{
		throw SystemException("cannot signal event (lock)");
	}
	m_state = true;
	if (pthread_cond_broadcast(&m_cond))
	{
		pthread_mutex_unlock(&m_mutex);
		throw SystemException("cannot signal event");
	}
	pthread_mutex_unlock(&m_mutex);
}


inline void EventImpl::resetImpl()
{
	if (pthread_mutex_lock(&m_mutex))
	{
		throw SystemException("cannot reset event");
	}
	m_state = false;
	pthread_mutex_unlock(&m_mutex);
}


}


#endif 

