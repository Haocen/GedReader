#include "Individual.h"

using namespace std;

Individual::Individual()
{
	this->deat = false;
}

Individual::Individual(string indi)
{
	this->indi = indi;
	this->deat = false;
}

Individual::~Individual()
{
}

Individual::Individual(string indi, string name, string given, string birthday, string sex, string surname, string marryname, string deathday)
{
	this->indi = indi;
	this->name = name;
	this->given = given;
	this->birthday = Date(birthday);
	this->sex = sex;
	this->surname = surname;
	this->marryname = marryname;
	if (deathday != string())
	{
		this->deat = true;
		this->deathday = Date(deathday);
	}
	else
	{
		this->deat = false;
	}
}