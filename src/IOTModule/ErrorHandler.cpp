//
// ErrorHandler.cpp
//


#include "ErrorHandler.h"
#include "SingletonHolder.h"


namespace CORE {


ErrorHandler* ErrorHandler::_pHandler = ErrorHandler::defaultHandler();
FastMutex ErrorHandler::_mutex;


ErrorHandler::ErrorHandler()
{
}


ErrorHandler::~ErrorHandler()
{
}


void ErrorHandler::exception(const Exception& exc)
{
}

	
void ErrorHandler::exception(const std::exception& exc)
{
}


void ErrorHandler::exception()
{
}


void ErrorHandler::handle(const Exception& exc)
{
	FastMutex::ScopedLock lock(_mutex);
	try
	{
		_pHandler->exception(exc);
	}
	catch (...)
	{
	}
}

	
void ErrorHandler::handle(const std::exception& exc)
{
	FastMutex::ScopedLock lock(_mutex);
	try
	{
		_pHandler->exception(exc);
	}
	catch (...)
	{
	}
}


void ErrorHandler::handle()
{
	FastMutex::ScopedLock lock(_mutex);
	try
	{
		_pHandler->exception();
	}
	catch (...)
	{
	}
}


ErrorHandler* ErrorHandler::set(ErrorHandler* pHandler)
{
	FastMutex::ScopedLock lock(_mutex);
	ErrorHandler* pOld = _pHandler;
	_pHandler = pHandler;
	return pOld;
}

	
ErrorHandler* ErrorHandler::defaultHandler()
{
	static SingletonHolder<ErrorHandler> sh;
	return sh.get();
}


} 

