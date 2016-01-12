#include "Date.h"
#include "Internal_Error.h"

using namespace std;

Date::Date()
{
	date = "1 JAN 0000";
	nodate = true;
}

Date::Date(string date)
{
	this->date = date;
	nodate = false;
}

Date::~Date()
{
}

void Date::SetDate(string date)
{
	this->date = date;
	nodate = false;
}

void Date::clear()
{
	date.clear();
	nodate = true;
}

string Date::GetDate()
{
	if (!nodate)
		return date;
	else
		return "NODATE";
}

int Date::GetDay() const
{
	int day = stoi(date.substr(0, date.find_first_of(" ")));
	if (nodate)
		throw Internal_Error("NODATE");
	else if (day >= 1 && day <= 31)
	{
		return day;
	}
	else
	{
		throw Internal_Error("Day of date exceeded limits.");
	}
}

int Date::GetMonth() const
{
	if (nodate)
		throw Internal_Error("NODATE");
	else if (date.find("JAN") != string::npos)
	{
		return 1;
	}
	else if (date.find("FEB") != string::npos)
	{
		return 2;
	}
	else if (date.find("MAR") != string::npos)
	{
		return 3;
	}
	else if (date.find("APR") != string::npos)
	{
		return 4;
	}
	else if (date.find("MAY") != string::npos)
	{
		return 5;
	}
	else if (date.find("JUN") != string::npos)
	{
		return 6;
	}
	else if (date.find("JUL") != string::npos)
	{
		return 7;
	}
	else if (date.find("AUG") != string::npos)
	{
		return 8;
	}
	else if (date.find("SEP") != string::npos)
	{
		return 9;
	}
	else if (date.find("OCT") != string::npos)
	{
		return 10;
	}
	else if (date.find("NOV") != string::npos)
	{
		return 11;
	}
	else if (date.find("DEC") != string::npos)
	{
		return 12;
	}
	else
	{
		throw Internal_Error("Month of date exceeded limits");
	}
}

int Date::GetYear() const
{
	int year = stoi(date.substr(date.find_last_of(" ") + 1));
	if (nodate)
		throw Internal_Error("NODATE");
	else if (year >= numeric_limits<int>::min() && year <= numeric_limits<int>::max())
	{
		return year;
	}
	else
	{
		throw Internal_Error("Year of date exceeded limits.");
	}
}

bool Date::operator==(const Date & obj)
{
	if (nodate || obj.nodate)
		return false;
	if (this->date == obj.date)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Date::operator!=(const Date & obj)
{
	return !(*this == obj);
}

bool Date::operator<(const Date & obj)
{
	if (nodate || obj.nodate)
		throw Internal_Error("NODATE");
	else if (this->GetYear() < obj.GetYear())
	{
		return true;
	}
	else if (this->validateMonth() && this->GetYear() == obj.GetYear() && this->GetMonth() < obj.GetMonth())
	{
		return true;
	}
	else if (this->validateDay() && this->GetYear() == obj.GetYear() && this->GetMonth() == obj.GetMonth() && this->GetDay() < obj.GetDay())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Date::operator<=(const Date & obj)
{
	if (nodate)
		throw Internal_Error("NODATE");
	else if (*this == obj)
	{
		return true;
	}
	else if (*this < obj)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Date::operator>(const Date & obj)
{
	if (nodate)
		throw Internal_Error("NODATE");
	else
		return !(*this <= obj);
}

bool Date::operator>=(const Date & obj)
{
	if (nodate)
		throw Internal_Error("NODATE");
	else
		return !(*this < obj);
}

ostream & operator<<(ostream & os, const Date & obj)
{
	if (obj.nodate)
	{
		os << "NODATE";
		return os;
	}
	else
	{
		os << obj.date;
		return os;
	}
}

istream & operator>>(istream & is, Date & obj)
{
	is >> obj.date;
	if (obj.validate())
	{
		is.setstate(std::ios::failbit);
		obj.nodate = true;
	}
	else
	{
		obj.nodate = false;
	}
	return is;
}

bool Date::validate()
{
	if (nodate)
		return false;
	try
	{
		this->GetYear();
	}
	catch (exception)
	{
		return false;
	}
	return true;
}

bool Date::validateMonth()
{
	if (nodate)
		return false;
	try
	{
		this->GetMonth();
	}
	catch (exception)
	{
		return false;
	}
	return true;
}

bool Date::validateDay()
{
	if (nodate)
		return false;
	try
	{
		this->GetDay();
	}
	catch (exception)
	{
		return false;
	}
	return true;
}