#pragma once
#include<vector>
#include<string>
#include "Person.h"
class Course;
class Professor:public Person
{
private:
	int departmentId;
	string specialization;
	vector<Course*>courses;
public:
	Professor(int id, string name, string email, int departmentId, string specialization);
	void addCourse(Course *newCourse);
	void displayInfo() const override;
	void displayCourses() const;
    bool havingCourse(int courseCode) override;

};
