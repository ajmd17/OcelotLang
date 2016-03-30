#ifndef EXCEPTION_TYPES_H
#define EXCEPTION_TYPES_H

#include "exception.h"

class AlreadyDeclaredException : public BaseException
{
public:
	AlreadyDeclaredException(std::string message) : BaseException(message)
	{

	}

	std::string getExceptionType() const
	{
		return "AlreadyDeclaredException";
	}
};

class UndeclaredException : public BaseException
{
public:
	UndeclaredException(std::string message) : BaseException(message)
	{

	}

	std::string getExceptionType() const
	{
		return "UndeclaredException";
	}
};

class BadSyntaxException : public BaseException
{
public:
	BadSyntaxException(std::string message) : BaseException(message)
	{

	}

	std::string getExceptionType() const
	{
		return "BadSyntaxException";
	}
};

class CastException : public BaseException
{
public:
	CastException(std::string message) : BaseException(message)
	{

	}

	std::string getExceptionType() const
	{
		return "CastException";
	}
};

class FormattingException : public BaseException
{
public:
	FormattingException(std::string message) : BaseException(message)
	{

	}

	std::string getExceptionType() const
	{
		return "FormattingException";
	}
};

#endif