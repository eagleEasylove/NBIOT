
#include "RWLock_WIN32.h"


namespace CORE {

/*
**	RWLockImpl: ��д��ʵ����
*/
RWLockImpl::RWLockImpl()
{
	m_readerCount = 0;
	// �ٽ����ĳ�ʼ��
	InitializeCriticalSectionAndSpinCount(&m_readLock, 4000);
	InitializeCriticalSectionAndSpinCount(&m_writeLock, 4000);
	InitializeCriticalSectionAndSpinCount(&m_readerCountLock, 4000);
}

RWLockImpl::~RWLockImpl()
{
	// ɾ���ٽ���
	DeleteCriticalSection(&m_readLock);
	DeleteCriticalSection(&m_writeLock); 
	DeleteCriticalSection(&m_readerCountLock);
}

//////////////////////////////////////////////////////////////////////////
// ����
void RWLockImpl::rdlockImpl()
{
	try
	{	
		EnterCriticalSection(&m_writeLock);		// ����д���ٽ���(��֤����д������ʱ���µĶ��߲����ٽ���)
		EnterCriticalSection(&m_readerCountLock); 
		
		++ m_readerCount;
		if( m_readerCount == 1 )
			EnterCriticalSection(&m_readLock);	// �ǵ�һ������ ��������ٽ���
		
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
			LeaveCriticalSection(&m_readLock);	// �����һ������ �뿪�����ٽ���
		}

		LeaveCriticalSection(&m_readerCountLock);
	}
	catch (...)
	{
		throw RWMutexException("cannot rdunlockImpl");
	}
}


//////////////////////////////////////////////////////////////////////////
// д��
void RWLockImpl::wrlockImpl()
{
	try
	{
		EnterCriticalSection(&m_writeLock);		// ����д���ٽ������Ƚ���д����
		EnterCriticalSection(&m_readLock);		// ��������ٽ�������֤���ж����˳���д�߲Ž��룩
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
		LeaveCriticalSection(&m_readLock);		// �뿪�����ٽ���
		LeaveCriticalSection(&m_writeLock);		// �뿪д���ٽ��������˳�д����
	}
	catch (...)
	{
		throw RWMutexException("cannot wrunlockImpl");
	}
}


//////////////////////////////////////////////////////////////////////////
// ���Լ���
bool RWLockImpl::tryrdlockImpl()
{
	try
	{
		if(TryEnterCriticalSection(&m_writeLock)) // ���Խ���д���ٽ���
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
		if( TryEnterCriticalSection(&m_writeLock) ) // ���Խ���д���ٽ���
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



