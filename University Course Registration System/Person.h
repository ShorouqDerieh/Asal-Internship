#pragma once
#include<string>
using namespace std;
class Person
{
	string name;
	int id;
	string email;
public:
	Person(int id, string name, string email);
    ~Person() = default;
	virtual void displayInfo() const;
	int getId() const;
	string getName() const;
	string getEmail() const;
	virtual bool havingCourse(int CourseCode) =0;
};
