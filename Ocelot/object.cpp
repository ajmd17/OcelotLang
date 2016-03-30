#include "object.h"

Object::Object()
{
	//this->refCount = 0;
}

Object::~Object()
{

}

/*unsigned short Object::getRefCount() const
{
	return this->refCount;
}

void Object::incrementRefCount()
{
	this->refCount++;
}

void Object::decrementRefCount()
{
	this->refCount--;
}*/

bool Object::equalTo(const Object &object)
{
	return false;
}