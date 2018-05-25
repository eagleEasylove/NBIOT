//
// RefCountedObject.cpp
//


#include "RefCountedObject.h"


namespace CORE {


RefCountedObject::RefCountedObject(): _rc(1)
{
}


RefCountedObject::~RefCountedObject()
{
}


void RefCountedObject::duplicate() const
{
	_rcMutex.lock();
	++_rc;
	_rcMutex.unlock();
}


void RefCountedObject::release() const
{
	_rcMutex.lock();
	Int32 rc = --_rc;
	_rcMutex.unlock();
	if (rc == 0) 
	{
		delete this;
	}
}


} 

