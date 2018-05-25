
//
//Rwlock_POSIX.h
//

#ifndef RWLock_POSIX_H
#define RWLock_POSIX_H

#include "../common_config_base.h"
#include "../Exception.h"
#include <pthread.h>
#include <errno.h>

namespace CORE{
	
/*******************************************************************************************
*linux环境下已经实现了读写锁的功能，在这里只是调用这些接口函数实现读写锁的最外层的用户封装
********************************************************************************************/
	class CORE_API RWLockImpl
	{
	protected:
		RWLockImpl();
		~RWLockImpl();
		
		void rdlockImpl();    // 读锁的实现；
		bool tryrdlockImpl(); // 尝试读锁的实现；
		void rdunlockImpl();  // 读锁的解除
		
		void wrlockImpl();    // 写锁的实现；
		bool trywrlockImpl(); // 尝试写锁的实现；
		void wrunlockImpl();  // 写锁的解除
	private:
		pthread_rwlock_t m_RWLock;
	};	
	
} 

#endif

