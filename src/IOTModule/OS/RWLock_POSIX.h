
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
*linux�������Ѿ�ʵ���˶�д���Ĺ��ܣ�������ֻ�ǵ�����Щ�ӿں���ʵ�ֶ�д�����������û���װ
********************************************************************************************/
	class CORE_API RWLockImpl
	{
	protected:
		RWLockImpl();
		~RWLockImpl();
		
		void rdlockImpl();    // ������ʵ�֣�
		bool tryrdlockImpl(); // ���Զ�����ʵ�֣�
		void rdunlockImpl();  // �����Ľ��
		
		void wrlockImpl();    // д����ʵ�֣�
		bool trywrlockImpl(); // ����д����ʵ�֣�
		void wrunlockImpl();  // д���Ľ��
	private:
		pthread_rwlock_t m_RWLock;
	};	
	
} 

#endif

