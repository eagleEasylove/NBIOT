//
// Mutex.h
//


#ifndef Mutex_H
#define Mutex_H


#include "common_config_base.h"
#include "ScopedLock.h"


#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/Mutex_WIN32.h"
#else
#include "OS/Mutex_POSIX.h"
#endif


namespace CORE {

/// Class Mutex
/// It is same as Lock
class CORE_API Mutex: private MutexImpl
{
public:
		/// Using the ScopedLock class is the preferred way to automatically
		/// lock and unlock a mutex.
	typedef _ScopedLock<Mutex> ScopedLock;
	typedef _ScopedUnLock<Mutex> ScopedUnLock;
		/// Constructor
	Mutex();

		/// Destructor
	~Mutex();

		/// Lock the mutex.
	void lock();

		/// Tries to lock the mutex.
		/// Return true if successfull,false otherwise.
		/// It returns immediately.
	bool tryLock();

		/// Unlock the mutex
	void unlock();
	
private:
	Mutex(const Mutex&);
	Mutex& operator = (const Mutex&);
};


class CORE_API FastMutex: private FastMutexImpl
{
public:
	typedef _ScopedLock<FastMutex> ScopedLock;
	typedef _ScopedUnLock<FastMutex> ScopedUnLock; 
	
	FastMutex();
		
	~FastMutex();

	void lock();

	bool tryLock();

	void unlock();
	
private:
	FastMutex(const FastMutex&);
	FastMutex& operator = (const FastMutex&);
};


//
// inlines
//
inline void Mutex::lock()
{
	lockImpl();
}


inline bool Mutex::tryLock()
{
	return tryLockImpl();
}


inline void Mutex::unlock()
{
	unlockImpl();
}


inline void FastMutex::lock()
{
	lockImpl();
}


inline bool FastMutex::tryLock()
{
	return tryLockImpl();
}


inline void FastMutex::unlock()
{
	unlockImpl();
}


} 


#endif 

