#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Internal_Error.h"
#include "People.h"
#include "Date.h"
//Code by Haocen.xu@gmail.com

class Family
{
protected:
	std::string fam;
	std::vector<std::shared_ptr<People>> husb;
	std::vector<std::shared_ptr<People>> wife;
	std::vector<std::shared_ptr<People>> chil;
	std::vector<std::string> notes;
	Date marr;
	Date div;
	bool current;

	bool AddMember(std::shared_ptr<People> people_ptr, std::vector<std::shared_ptr<People>> & group);
	bool RemoveMember(std::shared_ptr<People> people_ptr, std::vector<std::shared_ptr<People>> & group);
public:
	Family();
	Family(std::string fam);
	Family(std::string fam, std::shared_ptr<People> husb, std::shared_ptr<People> wife, std::shared_ptr<People> chil, std::string marr = std::string(), std::string div = std::string(), bool current = true);
	virtual ~Family();
	bool AddHusb(std::shared_ptr<People> people_ptr);
	bool RemoveHusb(std::shared_ptr<People> people_ptr);
	bool AddWife(std::shared_ptr<People> people_ptr);
	bool RemoveWife(std::shared_ptr<People> people_ptr);
	bool AddChil(std::shared_ptr<People> people_ptr);
	bool RemoveChil(std::shared_ptr<People> people_ptr);
	std::vector<std::shared_ptr<People>> & GetHusb();
	std::vector<std::shared_ptr<People>> & GetWife();
	std::vector<std::shared_ptr<People>> & GetChil();
	std::string GetID();
	void SetID(std::string id);
	Date GetMarr();
	void SetMarr(std::string marr);
	Date GetDiv();
	void SetDiv(std::string div);
	bool GetCurrent();
	void SetCurrent(bool current);
	void Clear();
	bool CheckSpousesExistenceAndOnlyOnePair();
	bool CheckDivorceBeforeMarriage();
	bool CheckDeathBeforeMarrige();
	bool CheckDeathBeforeDivorce();
	bool CheckMarriageBeforeBirth();
	bool CheckChildBeforeMarriageOrAfterDivorce();
	bool CheckMarriageBeforeLegal();
	bool CheckFamilyRecords();
	bool CheckImpossibleFather();
	std::vector<std::string> & GetNotes();
};