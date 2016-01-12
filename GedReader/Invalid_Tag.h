#pragma once
#include <exception>
#include <string>
//Code by Haocen.xu@gmail.com

class Invalid_Tag :
	public std::exception
{
public:
	Invalid_Tag();
	Invalid_Tag(const std::string & what_arg);
	virtual ~Invalid_Tag();
	virtual const char* what() const;
protected:
	std::string reason;
};