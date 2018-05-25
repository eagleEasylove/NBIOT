#ifndef RWLock_WIN32_H
#define RWLock_WIN32_H

#include "../common_config_base.h"
#include "../Exception.h"


namespace CORE {
	
	class CORE_API RWLockImpl
	{
		
	protected:
		RWLockImpl();
		~RWLockImpl();
		
		void rdlockImpl();
		bool tryrdlockImpl();
		void rdunlockImpl();
		
		void wrlockImpl();
		bool trywrlockImpl();
		void wrunlockImpl();
		
	private:
		unsigned int     m_readerCount;		// ���߼���
		CRITICAL_SECTION m_readLock;		// ����
		CRITICAL_SECTION m_writeLock;		// д��
		CRITICAL_SECTION m_readerCountLock; // ���߼������ʻ�����
	};
	
}



#endif

