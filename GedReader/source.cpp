#include <iostream>
#include <fstream>
#include <string>
#include "Invalid_Tag.h"
#include "Internal_Error.h"
#include "Individual.h"
#include "People.h"
#include "Family.h"
#include "Index.h"
#include "Date.h"

//Code by Haocen.xu@gmail.com

using namespace std;

int main()
{
	string file_path;
	int choice;
	ifstream load;
	vector<shared_ptr<Index>> index_vector;
	cout << "Welcome to GED reader." << endl;
	do
	{
		file_path.clear();
		choice = -1;
		cout << endl << endl;
		if (index_vector.size() != 0)
		{
			cout << "Current data is parsed form file: " << index_vector.back()->GetFilename() << endl;
		}
		cout << "Please input the number before the action you would like to perform:" << endl;
		cout << "1. Scan a new file." << endl;
		cout << "2. Display the scanned data." << endl;
		cout << "3. Output the scanned data." << endl;
		cout << "4. Display a report of possible errors." << endl;
		cout << "5. Save a report of possible errors to local storage." << endl;
		cout << "6. Remove the current data and restore the last scanned." << endl;
		cout << "7. Search for a specific people with his INDI." << endl;
		cout << "8. Search for a specific family with its FAM." << endl;
		cout << "0. Exit program." << endl;
		while (!(cin >> choice) || !(choice == 0 || choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7 || choice == 8))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please input the number before the intended action: ";
		};
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (index_vector.size() == 0 && choice != 1)
		{
			cout << "No file scanned yet." << endl;
			continue;
		}
		switch (choice)
		{
		case 1:
			cout << "Please enter either:" << endl << "1. File name(If under the same directory as this program)." << endl << "2. Full file path." << endl << "Warning: File MUST be coded in ANSI." << endl << "Otherwise program may not be able to read data properly." << endl << "Name/Path: ";
			while (!load.is_open())
			{
				getline(cin, file_path);
				if (file_path.find(".ged") == string::npos)
				{
					file_path += ".ged";
				}
				load.open(file_path);
				if (load.is_open())
					break;
				else
				{
					cout << "Cannot open specified file. An error occurred." << endl;
					cout << "Please enter either:" << endl << "1. File name(If under the same directory as this program)." << endl << "2. Full file path." << endl << "Warning: File MUST be coded in ANSI." << endl << "Otherwise program may not be able to read data properly." << endl << "Name/Path: ";
				}
			}
			index_vector.push_back(make_shared<Index>(file_path));
			index_vector.back()->Scanner(load);
			load.close();
			system("pause");
			break;
		case 2:
			index_vector.back()->Display();
			break;
		case 3:
		{
			cout << "Output will be saved to local storage in ged format." << endl;
			cout << "Warnning: TAGs not correctly parsed can not be saved." << endl;
			cout << "Please specify file name or full file path: ";
			while (!(cin >> file_path))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalide input. Please specify file name or full file path: ";
			};
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (file_path.find(".ged") == string::npos)
			{
				file_path = file_path + ".ged";
			}
			ifstream check(file_path);
			if (check.good())
			{
				check.close();
				cout << "File already exist." << endl << "To cancel input \'N\' or anything else to overwrite: ";
				char select = NULL;
				select = cin.get();
				if (select == 'N' || select == 'n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Saving canceled." << endl;
					continue;
				}
				else if (select != '\n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			}
			check.close();
			ofstream save;
			save.open(file_path, ios::trunc);
			if (!save.good())
			{
				cout << "Cannot save report, file error." << endl;
				continue;
			}
			try
			{
				index_vector.back()->Output(save);
			}
			catch (exception & obj)
			{
				cout << "An error occured. " << obj.what() << endl;
				cout << "Report file may not be complete." << endl;
			}
			cout << "File saved." << endl;
			save.close();
			system("pause");
			break;
		}
		case 4:
			index_vector.back()->Report(cout);
			system("pause");
			break;
		case 5:
		{
			cout << "Report will be saved to local storage in txt format." << endl;
			cout << "Please specify file name or full file path: ";
			while (!(cin >> file_path))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalide input. Please specify file name or full file path: ";
			};
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (file_path.find(".txt") == string::npos)
			{
				file_path = file_path + ".txt";
			}
			ifstream check(file_path);
			if (check.good())
			{
				check.close();
				cout << "File already exist." << endl << "To cancel input \'N\' or anything else to overwrite: ";
				char select = NULL;
				select = cin.get();
				if (select == 'N' || select == 'n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Saving canceled." << endl;
					continue;
				}
				else if (select != '\n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			}
			check.close();
			ofstream save;
			save.open(file_path, ios::trunc);
			if (!save.good())
			{
				cout << "Cannot save report, file error." << endl;
				continue;
			}
			try
			{
				index_vector.back()->Report(save);
			}
			catch (exception & obj)
			{
				cout << "An error occured. " << obj.what() << endl;
				cout << "Report file may not be complete." << endl;
			}
			cout << "Report saved." << endl;
			save.close();
			system("pause");
			break;
		}
		case 6:
			index_vector.pop_back();
			cout << "Successfully removed." << endl;
			system("pause");
			break;
		case 7:
		{
			string indi;
			cout << "Please specify the INDI: ";
			while (!(cin >> indi))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalide input. Please specify the INDI: ";
			};
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			try
			{
				shared_ptr<People> found;
				found = index_vector.back()->GetPeopleByID(indi);
				cout << "INDI: " << found->indi << endl;
				cout << "Name: " << found->name << endl;
				if (!found->given.empty())
				{
					cout << "Given: " << found->given << endl;
				}
				if (!found->surname.empty())
				{
					cout << "Surname: " << found->surname << endl;
				}
				if (!found->marryname.empty())
				{
					cout << "Marryname: " << found->marryname << endl;
				}
				cout << "Sex: " << found->sex << endl;
				if (found->birthday.validate())
				{
					cout << "Birthday: " << found->birthday << endl;
				}
				cout << "Deat: " << boolalpha << found->deat << endl;
				if (found->deat)
				{
					cout << "Deathday: " << found->deathday << endl;
				}
				for (auto iter = found->GetFams().cbegin(); iter != found->GetFams().cend(); ++iter)
				{
					cout << "Spouse of family: " << (*iter)->GetID() << endl;
				}
				for (auto iter = found->GetFamc().cbegin(); iter != found->GetFamc().cend(); ++iter)
				{
					cout << "Child of family: " << (*iter)->GetID() << endl;
				}
				for (auto iter = found->notes.cbegin(); iter != found->notes.cend(); ++iter)
				{
					cout << "Note: " << *iter << endl;
				}
			}
			catch (exception & obj)
			{
				cout << obj.what() << endl;
			}
			system("pause");
			break;
		}
		case 8:
		{
			string fam;
			cout << "Please specify the FAM: ";
			while (!(cin >> fam))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalide input. Please specify the FAM: ";
			};
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			try
			{
				shared_ptr<Family> found;
				found = index_vector.back()->GetFamilyByID(fam);
				cout << "FAM: " << found->GetID() << endl;
				for (auto iter = found->GetHusb().cbegin(); iter != found->GetHusb().cend(); ++iter)
				{
					cout << "Husband: " << (*iter)->indi << " " << (*iter)->name << endl;
				}
				for (auto iter = found->GetWife().cbegin(); iter != found->GetWife().cend(); ++iter)
				{
					cout << "Wife: " << (*iter)->indi << " " << (*iter)->name << endl;
				}
				for (auto iter = found->GetChil().cbegin(); iter != found->GetChil().cend(); ++iter)
				{
					cout << "Child: " << (*iter)->indi << " " << (*iter)->name << endl;
				}
				if (found->GetMarr().validate())
				{
					cout << "Marryday: " << found->GetMarr() << endl;
				}
				cout << "Current: " << boolalpha << found->GetCurrent() << endl;
				if (!found->GetCurrent() && found->GetDiv().validate())
				{
					cout << "Divorceday: " << found->GetDiv() << endl;
				}
				for (auto iter = found->GetNotes().cbegin(); iter != found->GetNotes().cend(); ++iter)
				{
					cout << "Note: " << *iter << endl;
				}
			}
			catch (exception & obj)
			{
				cout << obj.what() << endl;
			}
			system("pause");
			break;
		}
		default:
			break;
		}
	} while (choice != 0);
/*	cout << "Processing start." << endl;
	streampos last_line_pos;
	last_line_pos = load.tellg();
	while (getline(load, line))
	{
		//cout << line << endl; //DEBUG
		if (line.find("NOTE") != string::npos && *line.cbegin() == '0')
		{
			index_vector.back()->GetHead().push_back(line);
		}
		else if (line.find("TRLR") != string::npos && *line.cbegin() == '0')
		{
			if (index_vector.size() == 0 || index_vector.back()->GetHead().empty())
				cout << "This file end with no data parsed." << endl;
			else
			{
				index_vector.back()->GetHead().push_back(line);
				cout << "Reached end of the file." << endl;
				break;
			}
		}
		else if (line.find("INDI") != string::npos && *line.cbegin() == '0')
		{
			index_vector.back()->GetHead().push_back("PEOPLE_DATA");
			load.seekg(last_line_pos);
			try
			{
				index_vector.back()->PeopleScanner(load);
			}
			catch (exception & obj)
			{
				cout << obj.what() << endl;
				system("pause");
			}
		}
		else if (line.find("FAM") != string::npos && *line.cbegin() == '0')
		{
			index_vector.back()->GetHead().push_back("FAMILY_DATA");
			load.seekg(last_line_pos);
			try
			{
				index_vector.back()->FamilyScanner(load);
			}
			catch (exception & obj)
			{
				cout << obj.what() << endl;
				system("pause");
			}
		}
		else
		{
			index_vector.back()->GetHead().push_back(line);
		}
		last_line_pos = load.tellg();
	}
	cout << "Whole input file had been processed." << endl;*/
/*	{
		//Checking segment
		cout << "Now display internal data:" << endl;
		{
			cout << "Head data:" << endl;
			for (auto it = index_vector.back()->GetHead().cbegin(); it != index_vector.back()->GetHead().cend(); ++it)
			{
				cout << *it << endl;
			}
			system("pause");
			cout << "People data:" << endl;
			for (auto it = index_vector.back()->GetPeopleIndex().cbegin(); it != index_vector.back()->GetPeopleIndex().cend(); ++it)
			{
				cout << "INDI: " << (*it)->indi << endl;
				cout << "Name: " << (*it)->name << endl;
				cout << "Given: " << (*it)->given << endl;
				cout << "Surname: " << (*it)->surname << endl;
				cout << "Marryname: " << (*it)->marryname << endl;
				cout << "Sex: " << (*it)->sex << endl;
				cout << "Birthday: " << (*it)->birthday << endl;
				cout << "Deat: " << boolalpha << (*it)->deat << endl;
				cout << "Deathday: " << (*it)->deathday << endl;
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
		}
		{
			cout << "Family data:" << endl;
			for (auto it = index_vector.back()->GetFamilyIndex().cbegin(); it != index_vector.back()->GetFamilyIndex().cend(); ++it)
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
				cout << "Divorceday: " << (*it)->GetDiv() << endl;
				system("pause");
			}
			cout << "Checking segment end." << endl;
			system("pause");
		}
	}*/
	cout << "Program will now Exit." << endl;
	return 0;
}