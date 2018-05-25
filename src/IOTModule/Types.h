//
// Types.h
//



#if !defined(_TYPES_INCLUDED)
#define _TYPES_INCLUDED


#include "common_config_base.h"
#include <math.h>

namespace CORE {


#if defined(_MSC_VER)
	typedef signed char            	Int8;
	typedef unsigned char          	UInt8;
	typedef signed short           	Int16;
	typedef unsigned short         	UInt16;
	typedef signed int             	Int32;
	typedef unsigned int           	UInt32;
	typedef signed __int64         	Int64;
	typedef unsigned __int64       UInt64;
	typedef unsigned long			DWORD;
	typedef unsigned short			WORD;
//	typedef BOOL BOOL;
#elif defined(__GNUC__)
	//
	// Unix/GCC
	//
	typedef signed char            Int8;
	typedef unsigned char          UInt8;
	typedef signed short           Int16;
	typedef unsigned short         UInt16;
	typedef signed int             Int32;
	typedef unsigned int           UInt32;
	typedef signed long            IntPtr;
	typedef unsigned long          UIntPtr;
	typedef signed long long   Int64;
	typedef unsigned long long UInt64;
	typedef unsigned long			DWORD;
	typedef unsigned short			WORD;
	typedef int BOOL;
	#define TRUE 1
	#define FALSE 0
#endif


} 

namespace stdd
{
	typedef  long int     time_t;
#define FLOORR(x)  floor(x);
}


#endif 

