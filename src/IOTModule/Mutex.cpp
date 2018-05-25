//
// Mutex.cpp
//


#include "Mutex.h"

#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/Mutex_WIN32.cpp"
#else
#include "OS/Mutex_POSIX.cpp"
#endif


namespace CORE {


Mutex::Mutex()
{
}


Mutex::~Mutex()
{
}


FastMutex::FastMutex()
{
}


FastMutex::~FastMutex()
{
}


} 

