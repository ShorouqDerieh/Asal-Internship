#pragma once
#include<string>
using namespace std;
class Person {
	string name;
	int id;
	string email;
public:
	Person(int id, string name, string email);
	virtual void displayInfo();
	int getId();
	string getName();
	string getEmail();
	virtual bool havingCourse(int CourseCode) =0;
};