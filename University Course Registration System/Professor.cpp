#include <iostream>
#include<string>
#include "Professor.h"
#include "Course.h"
using namespace std;
Professor::Professor(int id, string name, string email, int departmentId, string specialization) :Person(id, name, email),
    departmentId(departmentId), specialization(specialization)
{
}
void Professor::addCourse(Course *newCourse)
{
	if (havingCourse(newCourse->getCourseCode()))
	{
		cout << "Already teaching course with code: " << newCourse->getCourseCode() << endl;
		return;
	}
	courses.push_back(newCourse);
}
void Professor::displayInfo() const
{
	cout << "Professor ID: " << getId() << endl
	     << "Name: " << getName() << endl
	     << "Department ID: " << departmentId << endl
	     << "Specialization: " << specialization << endl;
}	
void Professor::displayCourses() const
{
	if (courses.empty())
	{
		cout << "No courses assigned." << endl;
		return;
	}
	cout << "Courses taught by Professor:" << endl;
	for (Course *course : courses)
	{
		cout << "*" << course->getCourseName() << endl;
	}
}
bool Professor::havingCourse(int courseCode)
{
	for (Course *course : courses)
	{
		if (course->getCourseCode() == courseCode)
		{
			return true;
		}
	}
	return false;
}
