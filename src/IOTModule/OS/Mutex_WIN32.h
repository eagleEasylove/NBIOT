//
// Mutex_WIN32.h
//
 

#ifndef Mutex_WIN32_H
#define Mutex_WIN32_H

#include <windows.h>


#include "../common_config_base.h"
#include "../Exception.h"


namespace CORE {


class CORE_API MutexImpl
{
protected:
	
	MutexImpl();
	
	~MutexImpl();
	
	void lockImpl();
	
	bool tryLockImpl();
	
	void unlockImpl();
	
private:
	
	CRITICAL_SECTION m_cs;
	
};


typedef MutexImpl FastMutexImpl;


}


#endif 

