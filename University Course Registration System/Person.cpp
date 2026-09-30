#include "Person.h"
#include <iostream>
#include<string>
using namespace std;

Person::Person(int id, string name, string email) {
	this->id = id;
	this->name = name;
	this->email = email;
}
void Person::displayInfo() {
	cout << "Name: " << name << endl;
	cout << "ID: " << id << endl;
	cout << "Email: " << email << endl;
}
int Person::getId() {
	return id;
}
string Person::getName() {
	return name;
}
string Person::getEmail() {
	return email;
}
