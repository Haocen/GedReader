#pragma once
#include <exception>
#include <string>
//Code by Haocen.xu@gmail.com

class Internal_Error :
	public std::exception
{
public:
	Internal_Error();
	Internal_Error(const std::string & what_arg);
	virtual ~Internal_Error();
	virtual const char* what() const;
protected:
	std::string reason;
};
