#include "Internal_Error.h"


Internal_Error::Internal_Error()
{
	reason = "Internal Error found!";
}


Internal_Error::~Internal_Error()
{
}

Internal_Error::Internal_Error(const std::string & what_arg)
{
	this->reason = what_arg;
}

const char* Internal_Error::what() const
{
	const char* temp = reason.c_str();
	return temp;
}