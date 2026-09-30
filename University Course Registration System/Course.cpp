#include "Course.h"
#include<iostream>
#include "Student.h"
using namespace std;
Course::Course(int code, string name, int hours, int maxStudents) {
	courseCode = code;
	courseName = name;
	creditHours = hours;
	this->maxStudents = maxStudents;
}
void Course::displayCourseInfo(){
	cout << "Course Code: "<<courseCode<<endl;
	cout << "Course Name: "<<courseName<<endl;
	cout << "Credit Hours: "<<creditHours<<endl;
	cout << "Maximum Students: "<<maxStudents<<endl;
	cout << "Available Seats: " << maxStudents - enrolledStudents.size() << endl;
}
void Course::addStudent(Student *newStudent) {
	if (!isFull())
	{
		enrolledStudents.push_back(newStudent);
	}
	else {
		cout << "Cannot add student. Course is full." << endl;
	}
}
void Course::removeStudent(int studentId)
{
	if (enrolledStudents.empty())
	{
		cout << "No students enrolled in this course." << endl;
		return;
	}
	for (int i=0;i<enrolledStudents.size();i++)
	{
		if (enrolledStudents[i]->getId() == studentId)
		{
			enrolledStudents.erase(enrolledStudents.begin() + i);
			//cout << "Student removed from the course." << endl;
			break;
		}
	}
}
bool Course::isFull() {
	return enrolledStudents.size() >= maxStudents;
}
int  Course::getCourseCode() {
	return courseCode;
}
string  Course::getCourseName() {
	return courseName;
}