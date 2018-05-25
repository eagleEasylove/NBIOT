//
// ScopedLock.h
//


#ifndef ScopedLock_H
#define ScopedLock_H


#include "common_config_base.h"


namespace CORE {

/// Class _ScopedLock
/// Automatically lock mutex when constructor
/// Automatically unlock mutex when destructor
template <class M>
class _ScopedLock
{
public:
	inline _ScopedLock(M& mutex): m_mutex(mutex)
	{
		m_mutex.lock();
	}
	inline ~_ScopedLock()
	{
		m_mutex.unlock();
	}

private:
	M& m_mutex;

	_ScopedLock();
	_ScopedLock(const _ScopedLock&);
	_ScopedLock& operator = (const _ScopedLock&);
};


/// Class _ScopedUnLock
/// Automatically unlock mutex when constructor
/// Automatically lock mutex when destructor
template <class M>
class _ScopedUnLock
{
public:
	inline _ScopedUnLock(M& mutex): m_mutex(mutex)
	{
		m_mutex.unlock();
	}
	inline ~_ScopedUnLock()
	{
		m_mutex.lock();
	}

private:
	M& m_mutex;

	_ScopedUnLock();
	_ScopedUnLock(const _ScopedUnLock&);
	_ScopedUnLock& operator = (const _ScopedUnLock&);
};


} 


#endif 

