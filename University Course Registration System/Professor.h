#pragma once
#include "Person.h"
//#include "Course.h"
class Course;
#include<vector>
#include<string>
class Professor:public Person
{
private:
	int departmentId;
	string specialization;
	vector<Course*>courses;
public:
	Professor(int id, string name, string email, int departmentId, string specialization);
	void addCourse(Course *newCourse);
	void displayInfo() override;
	void displayCourses();
	bool havingCourse(int courseCode) override;

};
