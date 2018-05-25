//
// Event.cpp
//


#include "Event.h"


#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/Event_WIN32.cpp"
#else
#include "OS/Event_POSIX.cpp"
#endif


namespace CORE {


Event::Event(bool autoReset): EventImpl(autoReset)
{
}


Event::~Event()
{
}


} 

