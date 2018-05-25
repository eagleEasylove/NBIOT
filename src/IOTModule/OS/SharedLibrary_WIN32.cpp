//
// SharedLibrary_WIN32.cpp
//


#include "SharedLibrary_WIN32.h"
#include "../Logger.h"

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
		core_throw(LibraryAlreadyLoadedException(_path));
	}
	_handle = LoadLibraryA(path.c_str());
	if (!_handle) 
	{
		core_throw(LibraryLoadException(path));
	}
	_path = path;
}


void SharedLibraryImpl::unloadImpl()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle)
	{
		FreeLibrary((HMODULE) _handle);
		_handle = 0;
	}
	_path.erase();//.clear();
}


bool SharedLibraryImpl::isLoadedImpl() const
{
	return _handle != 0; 
}


void* SharedLibraryImpl::findSymbolImpl(const CData& name)
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle)
	{
		return (void*) GetProcAddress((HMODULE) _handle, name.c_str());
	}
	else return 0;
}


const CData& SharedLibraryImpl::getPathImpl() const
{
	return _path;
}


CData SharedLibraryImpl::suffixImpl()
{
#if defined(_DEBUG)
	return "d.dll";
#else
	return ".dll";
#endif
}


} 

