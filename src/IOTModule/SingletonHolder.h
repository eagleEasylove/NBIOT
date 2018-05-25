//
// SingletonHolder.h
//


#ifndef SingletonHolder_H
#define SingletonHolder_H


#include "common_config_base.h"
#include "Mutex.h"


namespace CORE {


template <class S>
class SingletonHolder
{
public:
	SingletonHolder()
	{
		_pS = 0;
	}
	~SingletonHolder()
	{
		delete _pS;
	}
	S* get()
	{
		if(_pS == 0)
		{
		//Modified by chen.yinggui@zte.com.cn 2010/6/30
		//For Double Checked Lock(DCL)
			FastMutex::ScopedLock lock(_m);
			if (!_pS) 
			{
				_pS = new S;
			}	
		}
		return _pS;
	}
	
private:
	S* _pS;
	FastMutex _m;
};


} 


#endif 

