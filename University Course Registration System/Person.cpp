#include "Person.h"
#include <iostream>
#include<string>
using namespace std;
Person::Person(int id, string name, string email):
    id(id), name(name), email(email)
{
}
void Person::displayInfo() const{
	cout << "Name: " << name << endl
	     <<"ID: " << id << endl
	     <<"Email: " << email << endl;
}
int Person::getId() const
{
	return id;
}
string Person::getName() const
{
	return name;
}
string Person::getEmail() const
{
	return email;
}
