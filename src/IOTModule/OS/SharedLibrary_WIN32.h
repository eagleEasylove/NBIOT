//
// SharedLibrary_WIN32.h
//


#ifndef SharedLibrary_WIN32_H
#define SharedLibrary_WIN32_H


#include "../common_config_base.h"
#include "../Mutex.h"


namespace CORE {


class CORE_API SharedLibraryImpl
{
protected:
	
	SharedLibraryImpl();
	
	~SharedLibraryImpl();
	
	void loadImpl(const CData& path);
	
	void unloadImpl();
	
	bool isLoadedImpl() const;
	
	void* findSymbolImpl(const CData& name);
	
	const CData& getPathImpl() const;
	
	static CData suffixImpl();

private:
	CData _path;
	void* _handle;
	static FastMutex _mutex;
};


}


#endif 

