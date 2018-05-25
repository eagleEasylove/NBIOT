//
// RefCountedObject.h
//


#ifndef RefCountedObject_H
#define RefCountedObject_H


#include "common_config_base.h"
#include "Mutex.h"


namespace CORE {

/// Class RefCountedObject
/// Reference-counted objects.
class CORE_API RefCountedObject
{
public:
	RefCountedObject();

		/// Increases the object's reference count.
	void duplicate() const;

		/// decreases the object's reference count.
	void release() const;

		/// Returns the object's reference count.
	Int32 referenceCount() const;

protected:
	virtual ~RefCountedObject();

private:
	RefCountedObject(const RefCountedObject&);
	RefCountedObject& operator = (const RefCountedObject&);

	mutable Int32 _rc;
	mutable FastMutex _rcMutex;
};


//
// inlines
//
inline Int32 RefCountedObject::referenceCount() const
{
	return _rc;
}


}


#endif 

