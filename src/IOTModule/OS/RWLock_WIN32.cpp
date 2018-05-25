
#include "RWLock_WIN32.h"


namespace CORE {

/*
**	RWLockImpl: 读写锁实现类
*/
RWLockImpl::RWLockImpl()
{
	m_readerCount = 0;
	// 临界区的初始化
	InitializeCriticalSectionAndSpinCount(&m_readLock, 4000);
	InitializeCriticalSectionAndSpinCount(&m_writeLock, 4000);
	InitializeCriticalSectionAndSpinCount(&m_readerCountLock, 4000);
}

RWLockImpl::~RWLockImpl()
{
	// 删除临界区
	DeleteCriticalSection(&m_readLock);
	DeleteCriticalSection(&m_writeLock); 
	DeleteCriticalSection(&m_readerCountLock);
}

//////////////////////////////////////////////////////////////////////////
// 读锁
void RWLockImpl::rdlockImpl()
{
	try
	{	
		EnterCriticalSection(&m_writeLock);		// 进入写锁临界区(保证当有写者请求时，新的读者不能再进入)
		EnterCriticalSection(&m_readerCountLock); 
		
		++ m_readerCount;
		if( m_readerCount == 1 )
			EnterCriticalSection(&m_readLock);	// 是第一个读者 进入读锁临界区
		
		LeaveCriticalSection(&m_readerCountLock);
		LeaveCriticalSection(&m_writeLock); 
	}
	catch (...)
	{
		throw RWMutexException("cannot rdlockImpl");
	}
}

void RWLockImpl::rdunlockImpl()
{
	try
	{
		EnterCriticalSection(&m_readerCountLock);

		-- m_readerCount;
		if( m_readerCount == 0 ) 
		{
			LeaveCriticalSection(&m_readLock);	// 是最后一个读者 离开读锁临界区
		}

		LeaveCriticalSection(&m_readerCountLock);
	}
	catch (...)
	{
		throw RWMutexException("cannot rdunlockImpl");
	}
}


//////////////////////////////////////////////////////////////////////////
// 写锁
void RWLockImpl::wrlockImpl()
{
	try
	{
		EnterCriticalSection(&m_writeLock);		// 进入写锁临界区（先进入写锁）
		EnterCriticalSection(&m_readLock);		// 进入读锁临界区（保证所有读者退出后写者才进入）
	}
	catch (...)
	{
		throw RWMutexException("cannot wrlockImpl");
	}
}


void RWLockImpl::wrunlockImpl()
{
	try
	{
		LeaveCriticalSection(&m_readLock);		// 离开读锁临界区
		LeaveCriticalSection(&m_writeLock);		// 离开写锁临界区（后退出写锁）
	}
	catch (...)
	{
		throw RWMutexException("cannot wrunlockImpl");
	}
}


//////////////////////////////////////////////////////////////////////////
// 尝试加锁
bool RWLockImpl::tryrdlockImpl()
{
	try
	{
		if(TryEnterCriticalSection(&m_writeLock)) // 尝试进入写锁临界区
		{
			EnterCriticalSection(&m_readerCountLock); 
			++ m_readerCount;
			if( m_readerCount == 1 )
				EnterCriticalSection(&m_readLock);
			LeaveCriticalSection(&m_readerCountLock);
			LeaveCriticalSection(&m_writeLock);
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
		throw RWMutexException("cannot tryrdlockImpl");
	}
}

bool RWLockImpl::trywrlockImpl()
{
	try
	{
		if( TryEnterCriticalSection(&m_writeLock) ) // 尝试进入写锁临界区
		{
			EnterCriticalSection(&m_readLock);
			return true;
		}
		else
			return false;
	}
	catch (...)
	{
		throw RWMutexException("cannot trywrlockImpl");
	}
}

} 



