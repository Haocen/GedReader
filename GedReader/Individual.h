#pragma once
#include <string>
#include <vector>
#include "Date.h"
//Code by Haocen.xu@gmail.com

class Individual
{
public:
	Individual();
	Individual(std::string indi);
	Individual(std::string indi, std::string name, std::string given, std::string birthday, std::string sex, std::string surname = std::string(), std::string marryname = std::string(), std::string deathday = std::string());
	virtual ~Individual();
	std::string indi;
	std::string name;
	std::string given;
	std::string surname;
	std::string marryname;
	std::string sex;
	Date birthday;
	Date deathday;
	bool deat;
	std::vector<std::string> notes;
};