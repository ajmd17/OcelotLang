#include "type_info.h"

TypeInfo::TypeInfo(std::string typeName, unsigned int size)
{
	this->typeName = typeName;
	this->size = size;
}

unsigned int TypeInfo::getSize()
{
	return this->size;
}

std::string TypeInfo::getTypeName()
{
	return this->typeName;
}