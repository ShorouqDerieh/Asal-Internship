#pragma once
#include<string>
#include<vector>
#include "Person.h"
//#include "Course.h"
class Course;
using namespace std;
class Student:public Person {
	string major;
	double gpa;
	vector<Course *> enrolledCourses;
public :
	Student(int id, string name, string email, string major, double gpa);
	void registerCourse(Course* newCourse);
	void dropCourse(int courseCode);
	void displayInfo() override;
	void displayRegisteredCourses();
	bool havingCourse(int courseCode) override;
};
