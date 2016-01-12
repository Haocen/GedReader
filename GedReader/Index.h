#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include "Internal_Error.h"
#include "Family.h"
#include "People.h"
//Code by Haocen.xu@gmail.com

class Index
{
protected:
	std::string filename;
	std::vector <std::string> head;
	std::vector <std::shared_ptr<Family>> family_index;
	std::vector <std::shared_ptr<People>> people_index;
	void PeopleScanner(std::ifstream & load);
	void FamilyScanner(std::ifstream & load);
public:
	Index();
	Index(std::string filename);
	std::string GetFilename();
	std::vector<std::string> & GetHead();
	std::vector<std::shared_ptr<Family>> & GetFamilyIndex();
	std::vector<std::shared_ptr<People>> & GetPeopleIndex();
	std::shared_ptr<Family> GetFamilyByID(std::string id);
	std::shared_ptr<People> GetPeopleByID(std::string id);
	void Scanner(std::ifstream & load);
	void Display();
	std::ostream & Output(std::ostream & os);
	std::ostream & Report(std::ostream & os);
	virtual ~Index();
};
