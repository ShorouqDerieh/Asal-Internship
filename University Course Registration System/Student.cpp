#include<iostream>
#include<string>
#include "Student.h"
#include "Course.h"
using namespace std;
Student::Student(int id, string name, string email, string major, double gpa) :Person(id, name, email),major(major), gpa(gpa)
{
}
void Student::registerCourse(Course *newCourse)
{
	if (newCourse->isFull())
	{
		cout << "Course" <<newCourse->getCourseName()<<" is full." << endl;
		return;
	}
	if (havingCourse(newCourse->getCourseCode()))
	{
		cout << "Student " << this->getName() << " is already registered in " << newCourse->getCourseName() << endl;
		return;
	}
	enrolledCourses.push_back(newCourse);
	newCourse->addStudent(this);
}
void Student::dropCourse(int CourseCode)
{
	for (int i = 0; i < enrolledCourses.size(); i++)
	{
		if (havingCourse(CourseCode))
		{
			string name = enrolledCourses[i]->getCourseName();
			enrolledCourses[i]->removeStudent(this->getId());
			enrolledCourses.erase(enrolledCourses.begin() + i);
			cout << "Student "<<this->getName() <<" dropped "<<name << endl;
			return;
		}
	}
}
void Student::displayInfo() const
{
	cout << "Student ID: " << getId() << endl
	     << "Name: " << getName() << endl
	     << "Email: " << getEmail() << endl
	     << "Major: " << major << endl
	     << "GPA: " << gpa << endl;
}
void Student::displayRegisteredCourses() const
{
	if (enrolledCourses.empty())
	{
		cout << "No courses registered." << endl;
		return;
	}
	cout << "Registered Courses:" << endl;
	for (Course *course : enrolledCourses)
	{
		
		cout<<"*" << course->getCourseName() << endl;
	}
}
bool Student::havingCourse(int courseCode)
{
	for (int i = 0; i < enrolledCourses.size(); i++)
	{
		if (enrolledCourses[i]->getCourseCode() == courseCode)
		{
			return true;
		}
	}
	return false;
}
