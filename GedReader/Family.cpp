#include "Family.h"
#include "Internal_Error.h"

using namespace std;

Family::Family()
{
}

Family::Family(string fam)
{
	this->fam = fam;
}

Family::Family(string fam, shared_ptr<People> husb, shared_ptr<People> wife, shared_ptr<People> chil, string marr, string div, bool current)
{
	this->fam = fam;
	this->husb.push_back(husb);
	this->wife.push_back(wife);
	this->chil.push_back(chil);
	this->marr = Date(marr);
	this->div = Date(div);
	this->current = current;
}


Family::~Family()
{
}

bool Family::AddMember(shared_ptr<People> people_ptr, vector<shared_ptr<People>> & group)
{
	for (auto it = group.cbegin(); it != group.cend(); it++)
	{
		if (*it == people_ptr)
		{
			return false;
		}
	}
	group.push_back(people_ptr);
	return true;
}

bool Family::RemoveMember(shared_ptr<People> people_ptr, vector<shared_ptr<People>> & group)
{
	for (auto it = group.cbegin(); it != group.cend(); it++)
	{
		if (*it == people_ptr)
		{
			group.erase(it);
			return true;
		}
	}
	return false;
}

bool Family::AddHusb(shared_ptr<People> people_ptr)
{
	return AddMember(people_ptr, husb);
}

bool Family::RemoveHusb(shared_ptr<People> people_ptr)
{
	return RemoveMember(people_ptr, husb);
}

bool Family::AddWife(shared_ptr<People> people_ptr)
{
	return AddMember(people_ptr, wife);
}

bool Family::RemoveWife(shared_ptr<People> people_ptr)
{
	return RemoveMember(people_ptr, wife);
}

bool Family::AddChil(shared_ptr<People> people_ptr)
{
	return AddMember(people_ptr, chil);
}

bool Family::RemoveChil(shared_ptr<People> people_ptr)
{
	return RemoveMember(people_ptr, chil);
}

vector<shared_ptr<People>> & Family::GetHusb()
{
	return husb;
}

vector<shared_ptr<People>> & Family::GetWife()
{
	return wife;
}

vector<shared_ptr<People>> & Family::GetChil()
{
	return chil;
}

Date Family::GetMarr()
{
	return marr;
}

void Family::SetMarr(string marr)
{
	this->marr = Date(marr);
}

Date Family::GetDiv()
{
	return div;
}

void Family::SetDiv(string div)
{
	this->div = Date(div);
}

bool Family::GetCurrent()
{
	return current;
}

void Family::SetCurrent(bool current)
{
	this->current = current;
}

string Family::GetID()
{
	return fam;
}

void Family::SetID(string id)
{
	this->fam = id;
}

void Family::Clear()
{
	fam.clear();
	husb.clear();
	wife.clear();
	chil.clear();
	marr.clear();
	div.clear();
	current = bool();
}

bool Family::CheckSpousesExistenceAndOnlyOnePair()
{
	if (husb.size() == 1 && wife.size() == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Family::CheckDivorceBeforeMarriage()
{
	if (current)
	{
		return false;
	}
	else if (marr.validate() && div.validate() && marr > div)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Family::CheckDeathBeforeMarrige()
{
	if (!CheckSpousesExistenceAndOnlyOnePair())
	{
		return false;
	}
	if (husb[0]->deat == false && wife[0]->deat == false)
	{
		return false;
	}
	else if (husb[0]->deat == true && husb[0]->deathday.validate() && marr.validate() && husb[0]->deathday < marr)
	{
		return true;
	}
	else if (wife[0]->deat == true && wife[0]->deathday.validate() && marr.validate() && wife[0]->deathday < marr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Family::CheckDeathBeforeDivorce()
{
	if (!CheckSpousesExistenceAndOnlyOnePair())
	{
		return false;
	}
	if (husb[0]->deat == false && wife[0]->deat == false)
	{
		return false;
	}
	else if (husb[0]->deat == true && husb[0]->deathday.validate() && div.validate() && husb[0]->deathday < div)
	{
		return true;
	}
	else if (wife[0]->deat == true && wife[0]->deathday.validate() && div.validate() && wife[0]->deathday < div)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Family::CheckMarriageBeforeBirth()
{
	if (!CheckSpousesExistenceAndOnlyOnePair())
	{
		return false;
	}
	if (husb[0]->birthday.validate() && marr.validate() && husb[0]->birthday > marr)
	{
		return true;
	}
	else if (wife[0]->birthday.validate() && marr.validate() && wife[0]->birthday > marr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Family::CheckChildBeforeMarriageOrAfterDivorce()
{
	if (chil.size() == 0)
	{
		return false;
	}
	else
	{
		Date max_child_birthday, min_child_birthday;
		for (auto it = chil.cbegin(); it != chil.cend(); ++it)
		{
			if ((*it)->birthday.validate())
			{
				max_child_birthday = (*it)->birthday;
				min_child_birthday = (*it)->birthday;
				break;
			}
		}
		if (!max_child_birthday.validate())
			return false;
		for (auto it = chil.cbegin(); it != chil.cend(); ++it)
		{
			if ((*it)->birthday.validate() && (*it)->birthday > max_child_birthday)
			{
				max_child_birthday = (*it)->birthday;
			}
			if ((*it)->birthday.validate() && (*it)->birthday < min_child_birthday)
			{
				min_child_birthday = (*it)->birthday;
			}
		}
		if (marr.validate() && min_child_birthday < marr)
		{
			return true;
		}
		else if (div.validate() && max_child_birthday > div)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Family::CheckMarriageBeforeLegal()
{
	if (!CheckSpousesExistenceAndOnlyOnePair())
	{
		return false;
	}
	if (!marr.validate())
	{
		return false;
	}
	if (husb[0]->birthday.validate() && (marr.GetYear() - husb[0]->birthday.GetYear()) < 18)
	{
		return true;
	}
	if (wife[0]->birthday.validate() && (marr.GetYear() - wife[0]->birthday.GetYear()) < 15)
	{
		return true;
	}
	return false;
}

bool Family::CheckFamilyRecords()
{
	bool found = false;
	for (auto iter = husb.cbegin(); iter != husb.cend(); ++iter)
	{
		for (auto it = (*iter)->GetFams().cbegin(); it != (*iter)->GetFams().cend(); ++it)
		{
			if (fam == (*it)->GetID())
				found = true;
		}
	}
	if (!found)
	{
		return false;
	}
	found = false;
	for (auto iter = wife.cbegin(); iter != wife.cend(); ++iter)
	{
		for (auto it = (*iter)->GetFams().cbegin(); it != (*iter)->GetFams().cend(); ++it)
		{
			if (fam == (*it)->GetID())
				found = true;
		}
	}
	if (!found)
	{
		return false;
	}
	if (chil.size() != 0)
	{
		found = false;
		for (auto iter = chil.cbegin(); iter != chil.cend(); ++iter)
		{
			for (auto it = (*iter)->GetFamc().cbegin(); it != (*iter)->GetFamc().cend(); ++it)
			{
				if (fam == (*it)->GetID())
					found = true;
			}
		}
		if (!found)
		{
			return false;
		}
	}
	return true;
}

bool Family::CheckImpossibleFather()
{
	if (!CheckSpousesExistenceAndOnlyOnePair())
	{
		return false;
	}
	else
	{
		for (auto it = chil.cbegin(); it != chil.cend(); ++it)
		{
			if (husb[0]->deat && husb[0]->deathday.validate() && (*it)->birthday.validate())
			{
				if (((*it)->birthday.GetYear() - husb[0]->deathday.GetYear()) >= 2)
				{
					return true;
				}
				else if ((*it)->birthday.GetYear() == (husb[0]->deathday.GetYear() + 1) && ((*it)->birthday.GetMonth() + 12) - husb[0]->deathday.GetMonth() > 9)
				{
					return true;
				}
				else if ((*it)->birthday.GetYear() == (husb[0]->deathday.GetYear()) && (*it)->birthday.GetMonth() - husb[0]->deathday.GetMonth() > 9)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

vector<string> & Family::GetNotes()
{
	return notes;
}