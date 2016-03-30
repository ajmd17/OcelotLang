#ifndef EXCEPT_H
#define EXCEPT_H

#include <string>

class BaseException
{
public:
	BaseException(std::string message, int errorCode=0);

	std::string getMessage() const;

	int getErrorCode() const;

	virtual std::string getExceptionType() const;
private:
	std::string message;
	int errorCode;
};

#endif