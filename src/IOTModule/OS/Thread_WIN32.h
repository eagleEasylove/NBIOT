//
// Thread_WIN32.h
//


#ifndef Thread_WIN32_H
#define Thread_WIN32_H


#include "../common_config_base.h"
#include "../Runnable.h"

namespace CORE {


class CORE_API ThreadImpl
{
public:	
	enum Priority
	{
		PRIO_LOWEST_IMPL  = THREAD_PRIORITY_LOWEST,
		PRIO_LOW_IMPL     = THREAD_PRIORITY_BELOW_NORMAL,
		PRIO_NORMAL_IMPL  = THREAD_PRIORITY_NORMAL,
		PRIO_HIGH_IMPL    = THREAD_PRIORITY_ABOVE_NORMAL,
		PRIO_HIGHEST_IMPL = THREAD_PRIORITY_HIGHEST
	};

	ThreadImpl();	
	
	~ThreadImpl();

	void setPriorityImpl(Int32 prio);
	
	Int32 getPriorityImpl() const;
	
	void startImpl(Runnable& target);

	void stopImpl();

	void joinImpl();
	
	bool joinImpl(Int32 milliseconds);
	
	bool isRunningImpl() const;
	
	static void sleepImpl(Int32 milliseconds);
	
	static void yieldImpl();
	
	static ThreadImpl* currentImpl();
	void SetTid(UInt32 tid){};//guec 2011-1-14
protected:
#if defined(_DLL)
	static DWORD WINAPI entry(LPVOID pThread);
#else
	static unsigned __stdcall entry(void* pThread);
#endif

private:
	Runnable* 	m_pTarget;
	HANDLE    	m_thread;
	Int32       		m_nPrio;
	static DWORD 	m_currentKey;
};


//
// inlines
//
inline Int32 ThreadImpl::getPriorityImpl() const
{
	return m_nPrio;
}


inline void ThreadImpl::sleepImpl(Int32 milliseconds)
{
	Sleep(DWORD(milliseconds));
}


inline void ThreadImpl::yieldImpl()
{
	Sleep(0);
}


}


#endif 

