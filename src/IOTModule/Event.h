//
// Event.h
//


#ifndef Event_H
#define Event_H


#include "common_config_base.h"
#include "Exception.h"
//#include "Logger.h"

#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/Event_WIN32.h"
#else
#include "OS/Event_POSIX.h"
#endif


namespace CORE {

/// Class Event
/// An synchronization object
class CORE_API Event: private EventImpl
{
public:
		/// @Brief
		/// If autoReset is true, the event object  
		/// will automatically reset after a wait()
		/// successfully.
	Event(bool autoReset = true);

		/// @Brief
		/// Destroys the event.
	~Event();

		/// @Brief
		/// Signals the event object.
		/// If autoReset is true,only one thread 
		/// waiting for the event can resume execution.
		/// If autoReset is false,all threads waiting for 
		/// the event can resume exrcution.
		/// @Note
		/// This maybe throw exception.
	void set();

		/// @Brief
		/// Waits for the event to become signalled.
		/// @Note
		/// This maybe throw exception.
	void wait();

		/// @Brief
		/// Waits for the event to become signalled.
		/// Throws a TimeoutException if the event
		/// does not become signalled within the specified
		/// time interval.
		/// @Note
		/// This maybe throw exception.
	void wait(Int32 milliseconds);

		/// @Brief
		/// Waits for the event.
		/// Return true if the event become signalled
		/// within the time intervel, false otherwise.
		/// @Note
		/// This maybe throw exception.
	bool tryWait(Int32 milliseconds);

		/// @Brief
		/// Set the event to unsignalled state.
		/// @Note
		/// This maybe throw exception.
	void reset();
	
private:
	
	Event(const Event&);
	
	Event& operator = (const Event&);
};


//
// inlines
//
inline void Event::set()
{
	setImpl();
}


inline void Event::wait()
{
	waitImpl();
}


inline void Event::wait(Int32 milliseconds)
{
	if (!waitImpl(milliseconds))
	{
		throw TimeoutException();
	}
}


inline bool Event::tryWait(Int32 milliseconds)
{
	return waitImpl(milliseconds);
}


inline void Event::reset()
{
	resetImpl();
}


} 


#endif 

