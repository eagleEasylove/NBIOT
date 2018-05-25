//
// Thread.cpp
//


#include "Thread.h"
#include "Mutex.h"
#include "Exception.h"
#include <sstream>


#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/Thread_WIN32.cpp"
#else
#include "OS/Thread_POSIX.cpp"
#endif


namespace CORE {


Thread::Thread(): 
	m_nId(0), 
	m_name(makeName())
{
#if!defined(CORE_OS_FAMILY_UNIX)
	m_nId=uniqueId();
#endif
}


Thread::Thread(const CData& name): 
	m_nId(0), 
	m_name(name)
{
#if!defined(CORE_OS_FAMILY_UNIX)	
	m_nId=uniqueId();
#endif
}


Thread::~Thread()
{

}


void Thread::setPriority(Priority prio)
{
	try{
		setPriorityImpl(prio);		
	}
	catch(...)
	{
	}
}


Thread::Priority Thread::getPriority() const
{
	return Priority(getPriorityImpl());
}


void Thread::start(Runnable& target)
{
	try{
		startImpl(target);	
	}
	catch(...)
	{
	}
}


void Thread::stop()
{
	stopImpl();
}


void Thread::join()
{	
	try{
		joinImpl();		
	}
	catch(...)
	{
	}
}


void Thread::join(Int32 milliseconds)
{
	try{
		
		if (!joinImpl(milliseconds))
		{
	//		_LogFatal<<"joinImpl()==false throw a Exception!"<<_LogFatalEnd;
			throw TimeoutException();
		}
	
	}
	catch(...)
	{
	}
	
}


bool Thread::tryJoin(Int32 milliseconds)
{
	try{	
		return joinImpl(milliseconds);
	}
	catch(...)
	{
		return false;
	}	
}


CData Thread::makeName()
{
	std::ostringstream name;
	name << '#' << m_nId;
	return name.str();
}


Int32 Thread::uniqueId()
{
	static FastMutex mtx;
	FastMutex::ScopedLock lock(mtx);

	static unsigned count = 0;
	++count;
	return count;
}


void Thread::setName(const CData& name)
{
	FastMutex::ScopedLock lock(m_mutex);

	m_name = name;
}


} 

