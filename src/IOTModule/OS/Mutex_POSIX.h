//
// Mutex_POSIX.h
//


#ifndef Mutex_POSIX_H
#define Mutex_POSIX_H


#include "../common_config_base.h"
#include "../Exception.h"
#include <pthread.h>
#include <errno.h>


namespace CORE {


class CORE_API MutexImpl
{
protected:
	MutexImpl();
	
	MutexImpl(bool fast);
	
	~MutexImpl();
	
	void lockImpl();
	
	bool tryLockImpl();
	
	void unlockImpl();
	
private:
	
	pthread_mutex_t m_mutex;
	
};


class CORE_API FastMutexImpl: public MutexImpl
{
protected:
	FastMutexImpl();
	~FastMutexImpl();
};


//
// inlines
//
inline void MutexImpl::lockImpl()
{
	if (pthread_mutex_lock(&m_mutex)) 
		throw MutexException("cannot lock mutex");
}


inline bool MutexImpl::tryLockImpl()
{
	Int32 rc = pthread_mutex_trylock(&m_mutex);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
	else
		throw MutexException("cannot lock mutex");

}


inline void MutexImpl::unlockImpl()
{
	if (pthread_mutex_unlock(&m_mutex))
		throw MutexException("cannot unlock mutex");
}


} 


#endif

