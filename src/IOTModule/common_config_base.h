#if !defined (_COMMON_CONFIG_BASE)
#define _COMMON_CONFIG__BASE

/**
Don't build DLL , it is used for test in cppuint
*/
#if defined(_WIN32)&&defined(_Test)
	#define CORE_API
#else

/**
build DLL
*/
#if defined(_WIN32) && defined(_DLL)
	#if !defined(COMMONMODULE_DLL) && !defined(COMMONMODULE_STATIC)
		#define COMMONMODULE_DLL
	#endif
#endif


#if defined(_WIN32) && defined(COMMONMODULE_DLL)
	#if defined(COMMONMODULE_EXPORTS)
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)	
	#endif
#endif


#if !defined(CORE_API)
	#define CORE_API
#endif


//
// Automatically link CORE library.
//
#if defined(_MSC_VER)
	#if !defined(CORE_NO_AUTOMATIC_LIBS) && !defined(COMMONMODULE_EXPORTS)
		#if defined(COMMONMODULE_DLL)
			#if defined(_DEBUG)
		//		#pragma comment(lib, "CommonModuled.lib")
			#else
		//		#pragma comment(lib, "CommonModule.lib")
			#endif
		#endif
	#endif
#endif // defined(_MSC_VER)
/**
build DLL end
*/
#endif // defined(_WIN32)&&defined(_Test)

#include "Platform.h"
#include "Types.h"
#include <string.h>	//c的定义
#include <string>	//c++的定义
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>	//针对INT_MAX的定义
#include <typeinfo>	//针对typeid的定义

#endif

