//
// Platform.h
//


#if !defined(_PLATFORM_H)
#define _PLATFORM_H


#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	#define CORE_OS_FAMILY_UNIX 1
#elif defined(_WIN32) 
	#define CORE_OS_FAMILY_WINDOWS 1
#endif

/********************  Win32 ***************************/	

#if defined(_MSC_VER) && !defined(_MT)
	#error Must compile with /MD, /MDd, /MT or /MTd
#endif


// Check debug/release settings consistency
#if defined(NDEBUG) && defined(_DEBUG)
	#error Inconsistent build settings (check for /MD[d])
#endif

// Window version
#if defined(_WIN32)
	#if !defined(_WIN32_WINNT)
		#define _WIN32_WINNT 0x0500
	#endif
#endif

// Turn off some annoying warnings
#if defined(_MSC_VER)
	#pragma warning(disable:4002) // signed/unsigned comparison
	#pragma warning(disable:4018) // signed/unsigned comparison
	#pragma warning(disable:4251) // ... needs to have dll-interface warning 
	#pragma warning(disable:4355) // 'this' : used in base member initializer list
	#pragma warning(disable:4996) // VC++ 8.0 deprecation warnings
	#pragma warning(disable:4351) // new behavior: elements of array '...' will be default initialized
	#pragma warning(disable:4675) // resolved overload was found by argument-dependent lookup
	#pragma warning(disable:4267) // size_t to int
	#pragma warning(disable:4275) // size_t to int
	#pragma warning(disable:4786) // identifier was truncated to '255' characters in the debug information
#endif

// Include 
#if defined(WIN32)

#define NOMINMAX

#if (_MSC_VER > 1200)	//vc7.1�ȸ߰汾
	#include <Winsock2.h>
	#include <WS2TCPIP.H>
	#include <windows.h>
#else	//vc6.0
	#include <windows.h>
	#include <Winsock2.h>
	#include <WS2TCPIP.H>
#endif	
	#pragma comment(lib, "Ws2_32.lib")
#endif

//
// Net
//
#if defined(CORE_OS_FAMILY_WINDOWS)
#define CORE_INVALID_SOCKET  INVALID_SOCKET
#define core_socket_t        SOCKET
#define core_socklen_t       int
#define core_closesocket(s)  closesocket(s)
#define CORE_EINTR           WSAEINTR
#define CORE_EACCES          WSAEACCES
#define CORE_EFAULT          WSAEFAULT
#define CORE_EINVAL          WSAEINVAL
#define CORE_EMFILE          WSAEMFILE
#define CORE_EAGAIN          WSAEWOULDBLOCK
#define CORE_EWOULDBLOCK     WSAEWOULDBLOCK
#define CORE_EINPROGRESS     WSAEINPROGRESS
#define CORE_EALREADY        WSAEALREADY
#define CORE_ENOTSOCK        WSAENOTSOCK
#define CORE_EDESTADDRREQ    WSAEDESTADDRREQ
#define CORE_EMSGSIZE        WSAEMSGSIZE
#define CORE_EPROTOTYPE      WSAEPROTOTYPE
#define CORE_ENOPROTOOPT     WSAENOPROTOOPT
#define CORE_EPROTONOSUPPORT WSAEPROTONOSUPPORT
#define CORE_ESOCKTNOSUPPORT WSAESOCKTNOSUPPORT
#define CORE_ENOTSUP         WSAEOPNOTSUPP
#define CORE_EPFNOSUPPORT    WSAEPFNOSUPPORT
#define CORE_EAFNOSUPPORT    WSAEAFNOSUPPORT
#define CORE_EADDRINUSE      WSAEADDRINUSE
#define CORE_EADDRNOTAVAIL   WSAEADDRNOTAVAIL
#define CORE_ENETDOWN        WSAENETDOWN
#define CORE_ENETUNREACH     WSAENETUNREACH
#define CORE_ENETRESET       WSAENETRESET
#define CORE_ECONNABORTED    WSAECONNABORTED
#define CORE_ECONNRESET      WSAECONNRESET
#define CORE_ENOBUFS         WSAENOBUFS
#define CORE_EISCONN         WSAEISCONN
#define CORE_ENOTCONN        WSAENOTCONN
#define CORE_ESHUTDOWN       WSAESHUTDOWN
#define CORE_ETIMEDOUT       WSAETIMEDOUT
#define CORE_ECONNREFUSED    WSAECONNREFUSED
#define CORE_EHOSTDOWN       WSAEHOSTDOWN
#define CORE_EHOSTUNREACH    WSAEHOSTUNREACH
#define CORE_ESYSNOTREADY    WSASYSNOTREADY
#define CORE_ENOTINIT        WSANOTINITIALISED
#define CORE_HOST_NOT_FOUND  WSAHOST_NOT_FOUND
#define CORE_TRY_AGAIN       WSATRY_AGAIN
#define CORE_NO_RECOVERY     WSANO_RECOVERY
#define CORE_NO_DATA         WSANO_DATA
#elif defined(CORE_OS_FAMILY_UNIX)
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <net/if.h>
#define CORE_INVALID_SOCKET  -1
#define core_socket_t        int
#define core_socklen_t       socklen_t
#define core_closesocket(s)  ::close(s)
#define CORE_EINTR           EINTR
#define CORE_EACCES          EACCES
#define CORE_EFAULT          EFAULT
#define CORE_EINVAL          EINVAL
#define CORE_EMFILE          EMFILE
#define CORE_EAGAIN          EAGAIN
#define CORE_EWOULDBLOCK     EWOULDBLOCK
#define CORE_EINPROGRESS     EINPROGRESS
#define CORE_EALREADY        EALREADY
#define CORE_ENOTSOCK        ENOTSOCK
#define CORE_EDESTADDRREQ    EDESTADDRREQ
#define CORE_EMSGSIZE        EMSGSIZE
#define CORE_EPROTOTYPE      EPROTOTYPE
#define CORE_ENOPROTOOPT     ENOPROTOOPT
#define CORE_EPROTONOSUPPORT EPROTONOSUPPORT
#define CORE_ESOCKTNOSUPPORT -1
#define CORE_ENOTSUP         ENOTSUP
#define CORE_EPFNOSUPPORT    EPFNOSUPPORT
#define CORE_EAFNOSUPPORT    EAFNOSUPPORT
#define CORE_EADDRINUSE      EADDRINUSE
#define CORE_EADDRNOTAVAIL   EADDRNOTAVAIL
#define CORE_ENETDOWN        ENETDOWN
#define CORE_ENETUNREACH     ENETUNREACH
#define CORE_ENETRESET       ENETRESET
#define CORE_ECONNABORTED    ECONNABORTED
#define CORE_ECONNRESET      ECONNRESET
#define CORE_ENOBUFS         ENOBUFS
#define CORE_EISCONN         EISCONN
#define CORE_ENOTCONN        ENOTCONN
#define CORE_ESHUTDOWN       -2
#define CORE_ETIMEDOUT       ETIMEDOUT
#define CORE_ECONNREFUSED    ECONNREFUSED
#define CORE_EHOSTDOWN       -3
#define CORE_EHOSTUNREACH    EHOSTUNREACH
#define CORE_ESYSNOTREADY    -4
#define CORE_ENOTINIT        -5
#define CORE_HOST_NOT_FOUND  HOST_NOT_FOUND
#define CORE_TRY_AGAIN       TRY_AGAIN
#define CORE_NO_RECOVERY     NO_RECOVERY
#define CORE_NO_DATA         NO_DATA
#endif

#ifndef INADDR_NONE
#define INADDR_NONE 0xFFFFFFFF
#endif



#endif 

