//
//Rwlock_POSIX.cpp
//

#include"RWLock_POSIX.h"

namespace CORE{
	
	RWLockImpl::RWLockImpl()
	{
		if(pthread_rwlock_init(&m_RWLock,NULL))
			throw RWMutexException("cannot create RWLock");
	}
	
	RWLockImpl::~RWLockImpl()
	{
		pthread_rwlock_destroy(&m_RWLock);
	}	
	
	
	void RWLockImpl::rdlockImpl()
	{
		if(pthread_rwlock_rdlock(& m_RWLock)) 
			throw RWMutexException("cannot rdlockImpl");
	}
	
	bool RWLockImpl::tryrdlockImpl()
	{
		int rc = pthread_rwlock_tryrdlock(& m_RWLock);
		if(rc == 0)
			return true;
		else if(rc == EBUSY)
			return false;
		else
			throw RWMutexException("cannot tryrdlockImpl");
	}
	
	void RWLockImpl::rdunlockImpl()
	{
		if(pthread_rwlock_unlock(& m_RWLock))
			throw RWMutexException("cannot rdunlockImpl");
	}
	
	void RWLockImpl::wrlockImpl()
	{
		if(pthread_rwlock_wrlock(& m_RWLock))
			throw RWMutexException("cannot wrlockImpl");
	}
	
	bool RWLockImpl::trywrlockImpl()
	{
		int rc = pthread_rwlock_trywrlock(& m_RWLock);
		if(rc == 0)
			return true;
		else if(rc == EBUSY)
			return false;
		else
			throw RWMutexException("cannot trywrlockImpl");
	}
	
	void  RWLockImpl::wrunlockImpl()
	{
		if(pthread_rwlock_unlock(& m_RWLock))
			throw RWMutexException("cannot wrunlockImpl");
	}
	
} 

