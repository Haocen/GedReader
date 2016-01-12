#include <ctime>
#include "People.h"
#include "Internal_Error.h"

using namespace std;

People::People()
{
}

People::People(string indi) :Individual(indi)
{
}

People::People(string indi, string name, string given, string birthday, string sex, string surname, string marryname, string deathday) :Individual(name, given, birthday, sex, surname, marryname, deathday)
{
}

People::People(Individual const & obj) : Individual(obj)
{
}

vector<shared_ptr<Family>> & People::GetFamc()
{
	return this->famc;
}

vector<shared_ptr<Family>> & People::GetFams()
{
	return this->fams;
}

People::~People()
{
}

bool People::CheckFamilyExsitence()
{
	if (this->GetFamc().size() > 0 || this->GetFams().size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool People::CheckDeathBeforeBirth()
{
	if (!deat)
	{
		return false;
	}
	else if (birthday.validate() && deathday.validate() && birthday > deathday)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool People::CheckChildOfMoreThanOneFamily()
{
	if (famc.size() > 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool People::CheckWalkingDead()
{
	if (!birthday.validate() || deat)
	{
		return false;
	}
	else
	{
		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);
		if ((now->tm_year + 1900) - birthday.GetYear() > 150)
		{
			return true; //Current date should be less than 150 years after birth for all living people
		}
		else
		{
			return false;
		}
	}
}