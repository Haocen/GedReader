#pragma once
#include <string>
//Code by Haocen.xu@gmail.com

class Date
{
private:
	std::string date;
	bool nodate;
public:
	Date();
	Date(std::string date);
	void SetDate(std::string date);
	int GetDay() const;
	int GetMonth() const;
	int GetYear() const;
	std::string GetDate();
	void clear();
	virtual ~Date();
	bool operator==(const Date & obj);
	bool operator!=(const Date & obj);
	bool operator<(const Date & obj);
	bool operator<=(const Date & obj);
	bool operator>(const Date & obj);
	bool operator>=(const Date & obj);
	bool validate();
	bool validateMonth();
	bool validateDay();

	friend std::ostream & operator<<(std::ostream & os, const Date & obj);
	friend std::istream & operator>>(std::istream & is, Date & obj);
};