#ifndef TYPE_INFO
#define TYPE_INFO

#include <string>

class TypeInfo
{
public:
	TypeInfo(std::string typeName = "undefined", unsigned int size = 1);

	std::string getTypeName();

	unsigned int getSize();
private:
	unsigned int size;
	std::string typeName;
};

#endif