//
// Event_WIN32.h
//


#ifndef Event_WIN32_H
#define Event_WIN32_H


#include "../common_config_base.h"
#include "../Exception.h"

namespace CORE {


class CORE_API EventImpl
{
protected:
	
	EventImpl(bool autoReset = false);	
	
	~EventImpl();
	
	void setImpl();
	
	void waitImpl();
	
	bool waitImpl(Int32 milliseconds);
	
	void resetImpl();
	
private:
	
	HANDLE m_event;
};


//
// inlines
//
inline void EventImpl::setImpl()
{
	if (!SetEvent(m_event))
	{
		throw SystemException("cannot signal event");
	}
}


inline void EventImpl::resetImpl()
{
	if (!ResetEvent(m_event))
	{
		throw SystemException("cannot reset event");
	}
}


}


#endif 

