#ifndef OBJECT_H
#define OBJECT_H

#include "type_info.h"
#include <memory>

class Object
{
private:
	//unsigned short refCount;
public:
	Object();

	~Object();

	/*unsigned short getRefCount() const;

	void incrementRefCount();

	void decrementRefCount();*/

	virtual bool equalTo(const Object &object);

	virtual TypeInfo getTypeInfo() = 0;
};

typedef std::shared_ptr<Object> ObjectPtr;

#endif