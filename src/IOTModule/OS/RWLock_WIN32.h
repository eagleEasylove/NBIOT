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
		unsigned int     m_readerCount;		// 读者计数
		CRITICAL_SECTION m_readLock;		// 读锁
		CRITICAL_SECTION m_writeLock;		// 写锁
		CRITICAL_SECTION m_readerCountLock; // 读者计数访问互斥锁
	};
	
}



#endif

