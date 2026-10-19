#pragma once
#include<string>
#include<vector>
#include "Person.h"
class Course;
using namespace std;
class Student:public Person
{
	string major;
	double gpa;
	vector<Course *> enrolledCourses;
public :
	Student(int id, string name, string email, string major, double gpa);
	void registerCourse(Course* newCourse);
	void dropCourse(int courseCode);
	void displayInfo() const override;
	void displayRegisteredCourses() const;
	bool havingCourse(int courseCode) override;
};
