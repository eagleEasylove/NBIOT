//
// SharedLibrary_UNIX.cpp
//


#include "SharedLibrary_UNIX.h"
#include "../Exception.h"
#include <dlfcn.h>


namespace CORE {


FastMutex SharedLibraryImpl::_mutex;


SharedLibraryImpl::SharedLibraryImpl()
{
	_handle = 0;
}


SharedLibraryImpl::~SharedLibraryImpl()
{
}


void SharedLibraryImpl::loadImpl(const CData& path)
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle) 
	{
		throw LibraryAlreadyLoadedException(path);
	}
	_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
	if (!_handle)
	{
		const char* err = dlerror();
		throw LibraryLoadException(err ? CData(err) : path);
	}
	_path = path;
}


void SharedLibraryImpl::unloadImpl()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle)
	{
		dlclose(_handle);
		_handle = 0;
	}
}


bool SharedLibraryImpl::isLoadedImpl() const
{
	return _handle != 0; 
}


void* SharedLibraryImpl::findSymbolImpl(const CData& name)
{
	FastMutex::ScopedLock lock(_mutex);

	void* result = 0;
	if (_handle)
	{
		result = dlsym(_handle, name.c_str());
	}
	return result;
}


const CData& SharedLibraryImpl::getPathImpl() const
{
	return _path;
}


CData SharedLibraryImpl::suffixImpl()
{
#if defined(_DEBUG)
		return "d.so";
#else
		return ".so";
#endif
}


} 

