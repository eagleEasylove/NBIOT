//
// Event_WIN32.cpp
//

#include "Event_WIN32.h"
#include "../Types.h"

namespace CORE {


EventImpl::EventImpl(bool autoReset)
{
	m_event = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
	if (!m_event)
	{
		throw SystemException("cannot create event");
	}
}


EventImpl::~EventImpl()
{
	CloseHandle(m_event);
}


void EventImpl::waitImpl()
{
	switch (WaitForSingleObject(m_event, INFINITE))
	{
		case WAIT_OBJECT_0:
		{
			return;
		}
		default:
		{
			throw SystemException("wait for event failed");
		}
	}
}

bool EventImpl::waitImpl(Int32 milliseconds)
{
	switch (WaitForSingleObject(m_event, milliseconds + 1))
	{
		case WAIT_TIMEOUT:
		{
			return false;
		}
		case WAIT_OBJECT_0:
		{
			return true;
		}
		default:
		{
			throw SystemException("wait for event failed");
		}
	}
}


}

