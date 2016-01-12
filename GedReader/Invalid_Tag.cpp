#include "Invalid_Tag.h"


Invalid_Tag::Invalid_Tag()
{
	reason = "Invalid Tag found!";
}


Invalid_Tag::~Invalid_Tag()
{
}

Invalid_Tag::Invalid_Tag(const std::string & what_arg)
{
	this->reason = what_arg;
}

const char* Invalid_Tag::what() const
{
	const char* temp = reason.c_str();
	return temp;
}