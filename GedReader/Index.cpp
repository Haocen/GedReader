#include "Index.h"

using namespace std;

Index::Index()
{
}

Index::~Index()
{
}

Index::Index(string filename)
{
	this->filename = filename;
}

string Index::GetFilename()
{
	return filename;
}

vector<string> & Index::GetHead()
{
	return head;
}

vector<shared_ptr<Family>> & Index::GetFamilyIndex()
{
	return family_index;
}

vector<shared_ptr<People>> & Index::GetPeopleIndex()
{
	return people_index;
}

shared_ptr<Family> Index::GetFamilyByID(string id)
{
	for (auto it = family_index.cbegin(); it != family_index.cend(); ++it)
	{
		if ((*it)->GetID() == id)
		{
			return *it;
		}
	}
	throw Internal_Error("Family not found.");
}

shared_ptr<People> Index::GetPeopleByID(string id)
{
	for (auto it = people_index.cbegin(); it != people_index.cend(); ++it)
	{
		if ((*it)->indi == id)
		{
			return *it;
		}
	}
	throw Internal_Error("People not found.");
}

void Index::PeopleScanner(ifstream & load)
{
	if (load.is_open())
	{
		string line;
		shared_ptr<People> people_ptr;
		streampos last_line_pos = load.tellg();
		cout << "Parsing people..." << endl;
		while (getline(load, line))
		{
			//cout << "Now parsing: " << line << endl; //DEBUG
			if (line.find("INDI") != string::npos && *line.cbegin() == '0')
			{
				//cout << "Find new people: " << line << endl; //DEBUG
				people_ptr = make_shared<People>(line.substr(line.find_first_of("@") + 1, line.find_last_of("@") - line.find_first_of("@") - 1));
				people_index.push_back(people_ptr);
			}
			else if (line.find("NAME") != string::npos && *line.cbegin() == '1')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				people_ptr->name = line.substr(line.find_first_of(" ", line.find("NAME") + 4) + 1);
			}
			else if (line.find("GIVN") != string::npos && *line.cbegin() == '2')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				people_ptr->given = line.substr(line.find_first_of(" ", line.find("GIVN") + 4) + 1);
			}
			else if (line.find("SURN") != string::npos && *line.cbegin() == '2')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				people_ptr->surname = line.substr(line.find_first_of(" ", line.find("SURN") + 4) + 1);
			}
			else if (line.find("_MARNM") != string::npos && *line.cbegin() == '2')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				people_ptr->marryname = line.substr(line.find_first_of(" ", line.find("_MARNM") + 6) + 1);
			}
			else if (line.find("SEX") != string::npos && *line.cbegin() == '1')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				people_ptr->sex = line.substr(line.find_first_of(" ", line.find("SEX") + 3) + 1);
			}
			else if (line.find("BIRT") != string::npos && *line.cbegin() == '1')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				if(getline(load, line))
					if (line.find("DATE ") != string::npos && *line.cbegin() == '2')
					{
						people_ptr->birthday = line.substr(line.find_first_of(" ", line.find("DATE") + 4) + 1);
					}
					else
					{
						throw Internal_Error("File not well formatted, DATE tag begin with 2 expected.");
					}
				else
					throw Internal_Error("File not complete, DATE tag expected.");
			}
			else if (line.find("DEAT") != string::npos && *line.cbegin() == '1')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				people_ptr->deat = true;
				last_line_pos = load.tellg();
				if (getline(load, line))
					if (line.find("DATE ") != string::npos && *line.cbegin() == '2')
					{
						people_ptr->deathday = line.substr(line.find_first_of(" ", line.find("DATE") + 4) + 1);
					}
					else
					{
						load.seekg(last_line_pos);
					}
				else
				{
					load.seekg(last_line_pos);
					//throw Internal_Error("File not complete, DATE tag expected.");
				}
			}
			else if (line.find("FAMS") != string::npos && *line.cbegin() == '1')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				string fams;
				fams = line.substr(line.find_first_of("@") + 1, line.find_last_of("@") - line.find_first_of("@") - 1);
				for (auto it = family_index.cbegin(); it != family_index.cend(); ++it)
				{
					if ((*it)->GetID() == fams)
					{
						people_ptr->GetFams().push_back(*it);
						break;
					}
				}
				if (people_ptr->GetFams().size() == 0 || people_ptr->GetFams().back()->GetID() != fams)
				{
					family_index.push_back(make_shared<Family>(fams));
					if (people_ptr->sex == "M")
						family_index.back()->AddHusb(people_ptr);
					else if (people_ptr->sex == "F")
						family_index.back()->AddWife(people_ptr);
					else
					{
						cout << "Sex not specified, assume husband in family." << endl;
						family_index.back()->AddHusb(people_ptr);
					}
					people_ptr->GetFams().push_back(family_index.back());
				}
				else
				{
					//Do nothing
				}
			}
			else if (line.find("FAMC") != string::npos && *line.cbegin() == '1')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				string famc;
				famc = line.substr(line.find_first_of("@") + 1, line.find_last_of("@") - line.find_first_of("@") - 1);
				for (auto it = family_index.cbegin(); it != family_index.cend(); ++it)
				{
					if ((*it)->GetID() == famc)
					{
						people_ptr->GetFamc().push_back(*it);
						break;
					}
				}
				if (people_ptr->GetFamc().size() == 0 || people_ptr->GetFamc().back()->GetID() != famc)
				{
					family_index.push_back(make_shared<Family>(famc));
					family_index.back()->AddChil(people_ptr);
					people_ptr->GetFamc().push_back(family_index.back());
				}
				else
				{
					//Do nothing
				}
			}
			else if (line.find("INDI") == string::npos && line.find("FAM") == string::npos && !line.empty() && *line.cbegin() != '0')
			{
				if (people_ptr == nullptr || people_ptr->indi.empty())
					throw Internal_Error("File not well formatted, people record should start with INDI.");
				else
				{
					cout << "Found TAG program cannot parse in the following line:" << endl << line << endl;
					people_ptr->notes.push_back(line);
					this->GetHead().push_back("PEOPLE_TAG_ERROR");
				}
			}
			else if (!line.empty() && *line.cbegin() == '0' && line.find("FAM")!=string::npos)
			{
				cout << "People parsing complete." << endl;
				load.seekg(last_line_pos);
				break;
			}
			else if (!line.empty() && *line.cbegin() == '0')
			{
				cout << "People parsing complete but file may contain more people data." << endl;
				load.seekg(last_line_pos);
				break;
			}
			else
			{
				cout << "Program failed to parse the following line:" << endl << line << endl;
				people_ptr->notes.push_back(line);
				this->GetHead().push_back("PEOPLE_TAG_ERROR");
				/*load.seekg(last_line_pos);
				break;*/
			}
			last_line_pos = load.tellg();
		}
	}
	else
		throw(Internal_Error("Cannot open file."));
}

void Index::FamilyScanner(ifstream & load)
{
	if (load.is_open())
	{
		string line;
		shared_ptr<Family> family_ptr;
		streampos last_line_pos = load.tellg();
		cout << "Parsing families..." << endl;
		while (getline(load, line))
		{
			//cout << line << endl; //DEBUG
			if (line.find("FAM") != string::npos && *line.cbegin() == '0')
			{
				string fam;
				fam = line.substr(line.find_first_of("@") + 1, line.find_last_of("@") - line.find_first_of("@") - 1);
				for (auto it = family_index.cbegin(); it != family_index.cend(); ++it)
				{
					if ((*it)->GetID() == fam)
					{
						family_ptr = *it;
						break;
					}
				}
				if (family_ptr == nullptr)
				{
					cout << "Found a family not related with any parsed people." << endl;
					family_ptr = make_shared<Family>(fam);
					family_index.push_back(family_ptr);
				}
			}
			else if (line.find("HUSB") != string::npos && *line.cbegin() == '1')
			{
				if (family_ptr==nullptr)
					throw Internal_Error("File not well formatted, family record should start with FAM.");
				string indi;
				indi = line.substr(line.find_first_of("@") + 1, line.find_last_of("@") - line.find_first_of("@") - 1);
				for (auto it = people_index.cbegin(); it != people_index.cend(); ++it)
				{
					//cout << "Compare " << (*it)->indi << " with " << indi << endl; //DEBUG
					if ((*it)->indi == indi)
					{
						family_ptr->AddHusb(*it);
						break;
					}
				}
				if (family_ptr->GetHusb().size() == 0 || family_ptr->GetHusb().back()->indi != indi)
				{
					cout << "Found a family husband not exist in parsed people." << endl;
					people_index.push_back(make_shared<People>(indi));
					//cout << "Try to fix by add: " << people_index.back()->indi << endl; //DEBUG
					family_ptr->AddHusb(people_index.back());
				}
				else
				{
					//Do nothing
				}
			}
			else if (line.find("WIFE") != string::npos && *line.cbegin() == '1')
			{
				if (family_ptr == nullptr)
					throw Internal_Error("File not well formatted, family record should start with FAM.");
				string indi;
				indi = line.substr(line.find_first_of("@") + 1, line.find_last_of("@") - line.find_first_of("@") - 1);
				for (auto it = people_index.cbegin(); it != people_index.cend(); ++it)
				{
					if ((*it)->indi == indi)
					{
						family_ptr->AddWife(*it);
						break;
					}
				}
				if (family_ptr->GetWife().size() == 0 || family_ptr->GetWife().back()->indi != indi)
				{
					cout << "Found a family wife not exist in parsed people." << endl;
					people_index.push_back(make_shared<People>(indi));
					family_ptr->AddWife(people_index.back());
				}
				else
				{
					//Do nothing
				}
			}
			else if (line.find("CHIL") != string::npos && *line.cbegin() == '1')
			{
				if (family_ptr == nullptr)
					throw Internal_Error("File not well formatted, family record should start with FAM.");
				string indi;
				indi = line.substr(line.find_first_of("@") + 1, line.find_last_of("@") - line.find_first_of("@") - 1);
				for (auto it = people_index.cbegin(); it != people_index.cend(); ++it)
				{
					if ((*it)->indi == indi)
					{
						family_ptr->AddChil(*it);
						break;
					}
				}
				if (family_ptr->GetChil().size()== 0 || family_ptr->GetChil().back()->indi != indi)
				{
					cout << "Found a family child not exist in parsed people." << endl;
					people_index.push_back(make_shared<People>(indi));
					family_ptr->AddChil(people_index.back());
				}
				else
				{
					//Do nothing
				}
			}
			else if (line.find("MARR") != string::npos && *line.cbegin() == '1')
			{
				if (family_ptr == nullptr)
					throw Internal_Error("File not well formatted, family record should start with FAM.");
				last_line_pos = load.tellg();
				if (getline(load, line))
				{
					if (line.find("DATE ") != string::npos && *line.cbegin() == '2')
					{
						family_ptr->SetMarr(line.substr(line.find_first_of(" ", line.find("DATE") + 4) + 1));
					}
					else
					{
						load.seekg(last_line_pos);
						//throw Internal_Error("File not well formatted, DATE tag begin with 2 expected.");
					}
				}
				else
				{
					load.seekg(last_line_pos);
					//throw Internal_Error("File not complete, DATE tag expected.");
				}
			}
			else if (line.find("DIV") != string::npos && *line.cbegin() == '1')
			{
				if (family_ptr == nullptr)
					throw Internal_Error("File not well formatted, family record should start with FAM.");
				family_ptr->SetCurrent(false);
				last_line_pos = load.tellg();
				if (getline(load, line))
				{
					if (line.find("DATE ") != string::npos && *line.cbegin() == '2')
					{
						family_ptr->SetDiv(line.substr(line.find_first_of(" ", line.find("DATE") + 4) + 1));
					}
					else
					{
						load.seekg(last_line_pos);
						//throw Internal_Error("File not well formatted, DATE tag begin with 2 expected.");
					}
				}
				else
				{
					load.seekg(last_line_pos);
					//throw Internal_Error("File not complete, DATE tag expected.");
				}
			}
			else if (line.find("_CURRENT") != string::npos && *line.cbegin() == '1')
			{
				if (family_ptr == nullptr)
					throw Internal_Error("File not well formatted, family record should start with FAM.");
				else if (line.find(" Y") != string::npos)
					family_ptr->SetCurrent(true);
				else if (line.find(" N") != string::npos)
					family_ptr->SetCurrent(false);
				else
					throw Internal_Error("Cannot parse current state of a family.");
			}
			else if (!line.empty() && line.find("TRLR") != string::npos && *line.cbegin() == '0')
			{
				cout << "Family parsing complete." << endl;
				load.seekg(last_line_pos);
				break;
			}
			else if (!line.empty() && *line.cbegin() == '0')
			{
				cout << "Family parsing complete but file may contain more family data." << endl;
				load.seekg(last_line_pos);
				break;
			}
			else
			{
				cout << "Program failed to parse the following line:" << endl << line << endl;
				family_ptr->GetNotes().push_back(line);
				this->GetHead().push_back("FAMILY_TAG_ERROR");
				/*load.seekg(last_line_pos);
				break;*/
			}
			last_line_pos = load.tellg();
		}
	}
}

void Index::Scanner(ifstream & load)
{
	cout << "Processing start." << endl;
	string line;
	streampos last_line_pos;
	last_line_pos = load.tellg();
	while (getline(load, line))
	{
		//cout << line << endl; //DEBUG
		if (line.find("NOTE") != string::npos && *line.cbegin() == '0')
		{
			this->GetHead().push_back(line);
		}
		else if (line.find("TRLR") != string::npos && *line.cbegin() == '0')
		{
			if (this->GetHead().empty())
			{
				this->GetHead().push_back("NO_DATA");
				cout << "This file end with no data parsed." << endl;
			}
			else
			{
				this->GetHead().push_back(line);
				cout << "Reached end of the file." << endl;
				break;
			}
		}
		else if (line.find("INDI") != string::npos && *line.cbegin() == '0')
		{
			this->GetHead().push_back("PEOPLE_DATA");
			load.seekg(last_line_pos);
			try
			{
				this->PeopleScanner(load);
			}
			catch (exception & obj)
			{
				this->GetHead().push_back("ERROR_PEOPLE_DATA: " + string(obj.what()));
				cout << obj.what() << endl;
				system("pause");
			}
		}
		else if (line.find("FAM") != string::npos && *line.cbegin() == '0')
		{
			this->GetHead().push_back("FAMILY_DATA");
			load.seekg(last_line_pos);
			try
			{
				this->FamilyScanner(load);
			}
			catch (exception & obj)
			{
				this->GetHead().push_back("ERROR_FAMILY_DATA: " + string(obj.what()));
				cout << obj.what() << endl;
				system("pause");
			}
		}
		else
		{
			this->GetHead().push_back(line);
		}
		last_line_pos = load.tellg();
	}
	cout << "Whole input file had been processed." << endl;
}

void Index::Display()
{
	cout << "Now displaying data in file: " + filename << " ." << endl;
	cout << "Head data:" << endl;
	for (auto it = this->GetHead().cbegin(); it != this->GetHead().cend(); ++it)
	{
		cout << *it << endl;
	}
	system("pause");
	cout << "People data:" << endl;
	for (auto it = this->GetPeopleIndex().cbegin(); it != this->GetPeopleIndex().cend(); ++it)
	{
		cout << "INDI: " << (*it)->indi << endl;
		cout << "Name: " << (*it)->name << endl;
		cout << "Given: " << (*it)->given << endl;
		cout << "Surname: " << (*it)->surname << endl;
		cout << "Marryname: " << (*it)->marryname << endl;
		cout << "Sex: " << (*it)->sex << endl;
		cout << "Birthday: " << (*it)->birthday << endl;
		cout << "Deat: " << boolalpha << (*it)->deat << endl;
		if ((*it)->deat)
		{
			cout << "Deathday: " << (*it)->deathday << endl;
		}
		for (auto iter = (*it)->GetFams().cbegin(); iter != (*it)->GetFams().cend(); ++iter)
		{
			cout << "Spouse of family: " << (*iter)->GetID() << endl;
		}
		for (auto iter = (*it)->GetFamc().cbegin(); iter != (*it)->GetFamc().cend(); ++iter)
		{
			cout << "Child of family: " << (*iter)->GetID() << endl;
		}
		for (auto iter = (*it)->notes.cbegin(); iter != (*it)->notes.cend(); ++iter)
		{
			cout << "Note: " << *iter << endl;
		}
		system("pause");
	}
	cout << "Family data:" << endl;
	for (auto it = this->GetFamilyIndex().cbegin(); it != this->GetFamilyIndex().cend(); ++it)
	{
		cout << "FAM: " << (*it)->GetID() << endl;
		for (auto iter = (*it)->GetHusb().cbegin(); iter != (*it)->GetHusb().cend(); ++iter)
		{
			cout << "Husband: " << (*iter)->indi << " " << (*iter)->name << endl;
		}
		for (auto iter = (*it)->GetWife().cbegin(); iter != (*it)->GetWife().cend(); ++iter)
		{
			cout << "Wife: " << (*iter)->indi << " " << (*iter)->name << endl;
		}
		for (auto iter = (*it)->GetChil().cbegin(); iter != (*it)->GetChil().cend(); ++iter)
		{
			cout << "Child: " << (*iter)->indi << " " << (*iter)->name << endl;
		}
		cout << "Marryday: " << (*it)->GetMarr() << endl;
		cout << "Current: " << boolalpha << (*it)->GetCurrent() << endl;
		if (!(*it)->GetCurrent())
		{
			cout << "Divorceday: " << (*it)->GetDiv() << endl;
		}
		for (auto iter = (*it)->GetNotes().cbegin(); iter != (*it)->GetNotes().cend(); ++iter)
		{
			cout << "Note: " << *iter << endl;
		}
		system("pause");
	}
	cout << "All data displayed." << endl;
}

ostream & Index::Output(ostream & os)
{
	bool put_family, put_people;
	put_family = put_people = false;
	for (auto it = this->GetHead().cbegin(); it != this->GetHead().cend(); ++it)
	{
		if ((*it) == "PEOPLE_DATA" && !put_people)
		{
			put_people = true;
			for (auto it = this->GetPeopleIndex().cbegin(); it != this->GetPeopleIndex().cend(); ++it)
			{
				os << "0 @" << (*it)->indi << "@ INDI" << endl;
				os << "1 NAME " << (*it)->name << endl;
				if (!(*it)->given.empty())
				{
					os << "2 GIVN " << (*it)->given << endl;
				}
				if (!(*it)->surname.empty())
				{
					os << "2 SURN " << (*it)->surname << endl;
				}
				if (!(*it)->marryname.empty())
				{
					os << "2 _MARNM " << (*it)->marryname << endl;
				}
				os << "1 SEX " << (*it)->sex << endl;
				if ((*it)->birthday.validate())
				{
					os << "1 BIRT" << endl << "2 DATE " << (*it)->birthday << endl;
				}
				if ((*it)->deat)
				{
					os << "1 DEAT Y" << endl << "2 DATE " << (*it)->deathday << endl;
				}
				for (auto iter = (*it)->GetFams().cbegin(); iter != (*it)->GetFams().cend(); ++iter)
				{
					os << "1 FAMS @" << (*iter)->GetID() << "@" << endl;
				}
				for (auto iter = (*it)->GetFamc().cbegin(); iter != (*it)->GetFamc().cend(); ++iter)
				{
					os << "1 FAMC @" << (*iter)->GetID() << "@" << endl;
				}
				/*for (auto iter = (*it)->notes.cbegin(); iter != (*it)->notes.cend(); ++iter)
				{
					os << "0 NOTE " << *iter << endl;
				}*/

			}
		}
		else if ((*it) == "FAMILY_DATA" && !put_family)
		{
			put_family = true;
			for (auto it = this->GetFamilyIndex().cbegin(); it != this->GetFamilyIndex().cend(); ++it)
			{
				os << "0 @" << (*it)->GetID() << "@ FAM" << endl;
				for (auto iter = (*it)->GetHusb().cbegin(); iter != (*it)->GetHusb().cend(); ++iter)
				{
					os << "1 HUSB @" << (*iter)->indi << "@" << endl;
				}
				for (auto iter = (*it)->GetWife().cbegin(); iter != (*it)->GetWife().cend(); ++iter)
				{
					os << "1 WIFE @" << (*iter)->indi << "@" << endl;
				}
				for (auto iter = (*it)->GetChil().cbegin(); iter != (*it)->GetChil().cend(); ++iter)
				{
					os << "1 CHIL @" << (*iter)->indi << "@" << endl;
				}
				if ((*it)->GetMarr().validate())
				{
					os << "1 MARR" << endl << "2 DATE " << (*it)->GetMarr() << endl;
				}
				if ((*it)->GetDiv().validate())
				{
					os << "1 DIV Y" << endl << "2 DATE " << (*it)->GetDiv() << endl;
				}
				if ((*it)->GetCurrent())
				{
					os << "1 _CURRENT Y" << endl;
				}
				else
				{
					os << "1 _CURRENT N" << endl;
				}
				/*for (auto iter = (*it)->GetNotes().cbegin(); iter != (*it)->GetNotes().cend(); ++iter)
				{
					os << "0 NOTE " << *iter << endl;
				}*/
			}
		}
		else if (*it == "PEOPLE_DATA" || *it == "FAMILY_DATA" || *it == "PEOPLE_TAG_ERROR" || *it == "FAMILY_TAG_ERROR" || *it == "NO_DATA")
		{
			//Do nothing
		}
		else if ((*it).find("ERROR_PEOPLE_DATA:") != string::npos || (*it).find("ERROR_FAMILY_DATA:") != string::npos)
		{
			//Do nothing
		}
		else
		{
			os << *it << endl;
		}
	}
	return os;
}

ostream & Index::Report(ostream & os)
{
	bool found = false;
	os << "Checking possible error in people." << endl;
	for (auto it = this->GetPeopleIndex().cbegin(); it != this->GetPeopleIndex().cend(); ++it)
	{
		if ((*it)->CheckChildOfMoreThanOneFamily() || (*it)->CheckDeathBeforeBirth() || !(*it)->CheckFamilyExsitence() || (*it)->CheckWalkingDead())
		{
			found = true;
			os << "INDI: " << (*it)->indi << endl;
			os << "Name: " << (*it)->name << endl;
			os << "Given: " << (*it)->given << endl;
			os << "Surname: " << (*it)->surname << endl;
			os << "Marryname: " << (*it)->marryname << endl;
			os << "Sex: " << (*it)->sex << endl;
			os << "Birthday: " << (*it)->birthday << endl;
			os << "Deat: " << boolalpha << (*it)->deat << endl;
			if ((*it)->deat)
			{
				os << "Deathday: " << (*it)->deathday << endl;
			}
			if ((*it)->CheckChildOfMoreThanOneFamily())
			{
				os << "Possible error: Child of more than one family." << endl;
			}
			if ((*it)->CheckDeathBeforeBirth())
			{
				os << "Possible error: Dead before born." << endl;
			}
			if (!(*it)->CheckFamilyExsitence())
			{
				os << "Possible error: Does not have a family." << endl;
			}
			if ((*it)->CheckWalkingDead())
			{
				os << "Possible error: Current date should be less than 150 years after birth for all living people." << endl;
			}
		}
		else
		{
			//Do nothing
		}
	}
	if (!found)
	{
		os << "No errors in people found." << endl;
	}
	found = false;
	os << "Checking possible error in families." << endl;
	for (auto it = this->GetFamilyIndex().cbegin(); it != this->GetFamilyIndex().cend(); ++it)
	{
		if ((*it)->CheckChildBeforeMarriageOrAfterDivorce() || (*it)->CheckDeathBeforeDivorce() || (*it)->CheckDeathBeforeMarrige() || (*it)->CheckDivorceBeforeMarriage() || (*it)->CheckMarriageBeforeBirth() || (*it)->CheckMarriageBeforeLegal() || !(*it)->CheckSpousesExistenceAndOnlyOnePair() || !(*it)->CheckFamilyRecords() || (*it)->CheckImpossibleFather())
		{
			found = true;
			os << "FAM: " << (*it)->GetID() << endl;
			for (auto iter = (*it)->GetHusb().cbegin(); iter != (*it)->GetHusb().cend(); ++iter)
			{
				os << "Husband: " << (*iter)->indi << " " << (*iter)->name << endl;
				os << "Birthday: " << (*iter)->birthday << endl;
				if ((*iter)->deat)
				{
					os << "Deathday: " << (*iter)->deathday << endl;
				}
			}
			for (auto iter = (*it)->GetWife().cbegin(); iter != (*it)->GetWife().cend(); ++iter)
			{
				os << "Wife: " << (*iter)->indi << " " << (*iter)->name << endl;
				os << "Birthday: " << (*iter)->birthday << endl;
				if ((*iter)->deat)
				{
					os << "Deathday: " << (*iter)->deathday << endl;
				}
			}
			for (auto iter = (*it)->GetChil().cbegin(); iter != (*it)->GetChil().cend(); ++iter)
			{
				os << "Child: " << (*iter)->indi << " " << (*iter)->name << endl;
				os << "Birthday: " << (*iter)->birthday << endl;
				if ((*iter)->deat)
				{
					os << "Deathday: " << (*iter)->deathday << endl;
				}
			}
			os << "Marryday: " << (*it)->GetMarr() << endl;
			os << "Current: " << boolalpha << (*it)->GetCurrent() << endl;
			if (!(*it)->GetCurrent())
			{
				os << "Divorceday: " << (*it)->GetDiv() << endl;
			}
			if ((*it)->CheckChildBeforeMarriageOrAfterDivorce())
			{
				os << "Possible error: Child born before marriage or after divorce." << endl;
			}
			if ((*it)->CheckDeathBeforeMarrige())
			{
				os << "Possible error: Married to a walking dead." << endl;
			}
			if ((*it)->CheckDeathBeforeDivorce())
			{
				os << "Possible error: Divorced with a walking dead." << endl;
			}
			if ((*it)->CheckDivorceBeforeMarriage())
			{
				os << "Possible error: Spouse form future want to divorce before marriage." << endl;
			}
			if ((*it)->CheckMarriageBeforeBirth())
			{
				os << "Possible error: Spouse form future want to marry before born." << endl;
			}
			if ((*it)->CheckMarriageBeforeLegal())
			{
				os << "Possible error: Spouse married before legal age." << endl;
			}
			if (!(*it)->CheckSpousesExistenceAndOnlyOnePair())
			{
				os << "Possible error: Currently only one man and one woman can legally marry." << endl;
			}
			if (!(*it)->CheckFamilyRecords())
			{
				os << "Possible error: People record and family record incosistency." << endl;
			}
			if ((*it)->CheckImpossibleFather())
			{
				os << "Possible error: Child should be born within 9 months after death of father." << endl;
			}
		}
	}
	if (!found)
	{
		os << "No errors in families found." << endl;
	}
	os << "Checking complete." << endl;
	return os;
}