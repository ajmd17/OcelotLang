#include "exception.h"

BaseException::BaseException(std::string message, int errorCode)
{
	this->message = message;
	this->errorCode = errorCode;
}

std::string BaseException::getMessage() const
{
	return this->message;
}

int BaseException::getErrorCode() const
{
	return this->errorCode;
}

std::string BaseException::getExceptionType() const
{
	return "BaseException";
}