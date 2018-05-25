//
// Thread.h
//


#ifndef Thread_H
#define Thread_H


#include "common_config_base.h"
#include "Mutex.h"
//#include "Logger.h"


#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/Thread_WIN32.h"
#else
#include "OS/Thread_POSIX.h"
#endif


namespace CORE {


class Runnable;

/// Class Thread
/// For threads
class CORE_API Thread: private ThreadImpl
{
public:	
	enum Priority
	{
		PRIO_LOWEST  = PRIO_LOWEST_IMPL, /// The lowest thread priority.
		PRIO_LOW     = PRIO_LOW_IMPL,    /// A lower than normal thread priority.
		PRIO_NORMAL  = PRIO_NORMAL_IMPL, /// The normal thread priority.
		PRIO_HIGH    = PRIO_HIGH_IMPL,   /// A higher than normal thread priority.
		PRIO_HIGHEST = PRIO_HIGHEST_IMPL /// The highest thread priority.
	};	
	
		/// @Brief
		/// Constructor.
		/// Call start() to start it.
	Thread();
	
		/// @Brief
		/// Creates a named thread.
	Thread(const CData& name);
	
		/// @Brief
		/// Destructor.
	~Thread();
	
		/// @Brief
		/// Returns the unique id of the thread.
	Int32 id();
	
		/// @Brief
		/// Returns the name of the thread.
	CData name() const;
	
		/// @Brief
		/// Return the name of the thread.
	CData getName() const;
	
		/// @Brief
		/// Sets the name of the thread.
	void setName(const CData& name);
	
		/// @Brief
		/// Sets the priority of the thread.
	void setPriority(Priority prio);
	
		/// @Brief
		/// Gets the priority of the thread.
	Priority getPriority() const;
	
		/// @Brief
		/// Starts the thread.
	void start(Runnable& target);

		/// @Brief
		/// Stops the thread.
		/// @Note
		/// You must do it as follow:
		///
		/// step 1.	stat(target1);
		/// step 2.	join();
		/// step 3.	stop();
		/// step 4.	stat(target2);
		/// ...
	void stop();
	
		/// @Brief
		/// Waits until the thread completes execution.
	void join();
		
	void join(Int32 milliseconds);
		
	bool tryJoin(Int32 milliseconds);
	
		/// @Brief
		/// Returns the state.
	bool isRunning() const;
	
		/// @Brief
		/// Sleep the current thread.
	static void sleep(Int32 milliseconds);
	
		/// @Brief
		/// yield cpu to the other threads.
	static void yield();
	
		/// @Brief
		/// Returns the Thread object for currently active thread.
	static Thread* current();

protected:
	
		/// @Brief
		/// Creates and returns a name for a thread.
	CData makeName();
	
		/// @Brief
		/// Creates and returns unique id for a thread
	static Int32 uniqueId();
		
private:
	
	Thread(const Thread&);
	
	Thread& operator = (const Thread&);

	Int32           m_nId;
	CData           m_name;
	mutable FastMutex	m_mutex;

	friend class PooledThread;
};


//
// inlines
//
inline Int32 Thread::id() 
{
	if(m_nId==0)
	{
#if defined(CORE_OS_FAMILY_UNIX)
		{
			m_nId = getTidImpl();
		}
#else
		m_nId = uniqueId();
#endif	
	}
	
	return m_nId;
}


inline CData Thread::name() const
{
	FastMutex::ScopedLock lock(m_mutex);
	
	return m_name;
}


inline CData Thread::getName() const
{
	FastMutex::ScopedLock lock(m_mutex);
	
	return m_name;
}


inline bool Thread::isRunning() const
{
	return isRunningImpl();
}


inline void Thread::sleep(Int32 milliseconds)
{
	sleepImpl(milliseconds);
}


inline void Thread::yield()
{
	yieldImpl();
}


inline Thread* Thread::current()
{
	return static_cast<Thread*>(currentImpl());
}


} 


#endif 

