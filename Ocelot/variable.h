#ifndef VARIABLE_H
#define VARIABLE_H

#include "object.h"
#include "base_type_names.h"

#include <string>
using std::string;

template <class T>
class Variable : public Object
{
private:
	T data;
public:
	Variable(T data=0)
	{
		this->data = data;
	}

	const T &getData() const
	{
		return this->data;
	}

	void setData(T data)
	{
		this->data = data;
	}

	virtual bool compatibleWith(const Variable &variable)
	{
		return false;
	}

	virtual bool equalTo(const Object &object)
	{
		const Variable<T> *otherObjectCast = dynamic_cast<const Variable<T>*>(&object);
		if (otherObjectCast != NULL)
		{
			if (sizeof(otherObjectCast) != sizeof(this))
				return false;
			else if (otherObjectCast->data == this->data)
				return true;
		}
		return false;
	}

	virtual string getTypeName() const = 0;

	TypeInfo getTypeInfo()
	{
		return TypeInfo(getTypeName(), sizeof(data));
	}
};

class IntVariable;
class BoolVariable;
class FloatVariable;
class DoubleVariable;
class StringVariable;

class BoolVariable : public Variable<bool>
{
public:
	BoolVariable(bool data) : Variable(data)
	{
	}

	operator int() const
	{
		return static_cast<int>(getData());
	}

	template <class T2>
	bool compatibleWith(const Variable<T2> &variable)
	{
		return false;
	}

	string getTypeName() const
	{
		return TYPE_BOOL;
	}
};

class StringVariable : public Variable<string>
{
public:

	StringVariable(string data) : Variable(data)
	{
	}

	template <class T2>
	bool compatibleWith(const Variable<T2> &variable)
	{
		return false;
	}

	bool compatibleWith(const StringVariable &variable)
	{
		return true;
	}

	string getTypeName() const
	{
		return TYPE_STRING;
	}
};

class IntVariable : public Variable<int>
{
public:

	IntVariable(int data) : Variable(data)
	{
	}

	operator float() const
	{
		return static_cast<float>(getData());
	}

	operator double() const
	{
		return static_cast<double>(getData());
	}

	operator long() const
	{
		return static_cast<long>(getData());
	}

	operator short() const
	{
		return static_cast<short>(getData());
	}

	operator unsigned int() const
	{
		return static_cast<unsigned int>(getData());
	}

	template <class T2>
	bool compatibleWith(const Variable<T2> &variable)
	{
		return false;
	}

	bool compatibleWith(const DoubleVariable &variable)
	{
		return true;
	}

	bool compatibleWith(const FloatVariable &variable)
	{
		return true;
	}

	bool compatibleWith(const IntVariable &variable)
	{
		return true;
	}

	string getTypeName() const
	{
		return TYPE_INT;
	}
};

class FloatVariable : public Variable<float>
{
public:

	FloatVariable(float data) : Variable(data)
	{
	}

	operator int() const
	{
		return static_cast<int>(getData());
	}
	
	operator double() const
	{
		return static_cast<double>(getData());
	}

	operator long() const
	{
		return static_cast<long>(getData());
	}

	operator short() const
	{
		return static_cast<short>(getData());
	}

	operator unsigned int() const
	{
		return static_cast<unsigned int>(getData());
	}

	template <class T2>
	bool compatibleWith(const Variable<T2> &variable)
	{
		return false;
	}

	bool compatibleWith(const FloatVariable &variable)
	{
		return true;
	}

	bool compatibleWith(const DoubleVariable &variable)
	{
		return true;
	}

	bool compatibleWith(const IntVariable &variable)
	{
		return true;
	}

	string getTypeName() const
	{
		return TYPE_FLOAT;
	}
};

class DoubleVariable : public Variable<double>
{
public:

	DoubleVariable(double data) : Variable(data)
	{
	}

	operator int() const
	{
		return static_cast<int>(getData());
	}

	operator float() const
	{
		return static_cast<float>(getData());
	}

	operator long() const
	{
		return static_cast<long>(getData());
	}

	operator short() const
	{
		return static_cast<short>(getData());
	}

	operator unsigned int() const
	{
		return static_cast<unsigned int>(getData());
	}

	template <class T2>
	bool compatibleWith(const Variable<T2> &variable)
	{
		return false;
	}

	bool compatibleWith(const DoubleVariable &variable)
	{
		return true;
	}

	bool compatibleWith(const FloatVariable &variable)
	{
		return true;
	}

	bool compatibleWith(const IntVariable &variable)
	{
		return true;
	}

	string getTypeName() const
	{
		return TYPE_DOUBLE;
	}
};


// Base types
/*
typedef Variable<std::string> StringVariable;
typedef Variable<float> FloatVariable;
typedef Variable<double> DoubleVariable;
typedef Variable<int> IntVariable;
typedef Variable<unsigned int> UintVariable;*/

#endif