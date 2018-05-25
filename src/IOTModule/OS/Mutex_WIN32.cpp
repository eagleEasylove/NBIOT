//
// Mutex_WIN32.cpp
//


#include "Mutex_WIN32.h"


namespace CORE {


MutexImpl::MutexImpl()
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
}


MutexImpl::~MutexImpl()
{
	DeleteCriticalSection(&m_cs);
}


void MutexImpl::lockImpl()
{
	try
	{
		EnterCriticalSection(&m_cs);
	}
	catch (...)
	{
		throw MutexException("cannot lock mutex");
	}
}


bool MutexImpl::tryLockImpl()
{
	try
	{
		return TryEnterCriticalSection(&m_cs) == TRUE;
	}
	catch (...)
	{
	}
	throw MutexException("cannot lock mutex");
}


void MutexImpl::unlockImpl()
{
	LeaveCriticalSection(&m_cs);
}



} 

