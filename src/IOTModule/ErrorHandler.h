//
// ErrorHandler.h
//


#ifndef ErrorHandler_H
#define ErrorHandler_H


#include "common_config_base.h"
#include "Exception.h"
#include "Mutex.h"
//#include "Logger.h"


namespace CORE {


class CORE_API ErrorHandler
{
public:
	ErrorHandler();

	virtual ~ErrorHandler();
	
	virtual void exception(const Exception& exc);
		
	virtual void exception(const std::exception& exc);

	virtual void exception();
		
	static void handle(const Exception& exc);
		
	static void handle(const std::exception& exc);
	
	static void handle();
	
	static ErrorHandler* set(ErrorHandler* pHandler);

	static ErrorHandler* get();

protected:	
	static ErrorHandler* defaultHandler();
		
private:
	static ErrorHandler* _pHandler;
	static FastMutex     _mutex;
};


//
// inlines
//
inline ErrorHandler* ErrorHandler::get()
{
	return _pHandler;
}


#define _ErrorHandler(x) \
	_LogFatal<<"Handle exception:"<<x.what()<<_LogFatalEnd \
	CORE::ErrorHandler::handle(x);

#define _ErrorHandlerNoParam() \
	_LogFatal<<"Don't know the exception"<<_LogFatalEnd \
	CORE::ErrorHandler::handle();
						
} 


#endif 

