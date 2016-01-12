#pragma once
#include "Individual.h"
#include <memory>
#include <vector>
//Code by Haocen.xu@gmail.com

class Family;

class People :
	public Individual
{
protected:
	std::vector<std::shared_ptr<Family>> famc;
	std::vector<std::shared_ptr<Family>> fams;
public:
	People();
	People(std::string indi);
	People(std::string indi, std::string name, std::string given, std::string birthday, std::string sex, std::string surname = std::string(), std::string marryname = std::string(), std::string deathday = std::string());
	People(Individual const & obj);
	std::vector<std::shared_ptr<Family>> & GetFamc();
	std::vector<std::shared_ptr<Family>> & GetFams();
	virtual ~People();
	bool CheckFamilyExsitence();
	bool CheckDeathBeforeBirth();
	bool CheckChildOfMoreThanOneFamily();
	bool CheckWalkingDead();
};