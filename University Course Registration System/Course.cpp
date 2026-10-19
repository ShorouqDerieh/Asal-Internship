#include "Course.h"
#include<iostream>
#include "Student.h"
using namespace std;
Course::Course(int code, string name, int hours, int maxStudents):
	courseCode(code), courseName(name), creditHours(hours), maxStudents(maxStudents)
{
}
void Course::displayCourseInfo() const
{
	cout << "Course Code: "<<courseCode<<endl
	     << "Course Name: "<<courseName<<endl
	     << "Credit Hours: "<<creditHours<<endl
	     << "Maximum Students: "<<maxStudents<<endl
	     << "Available Seats: " << maxStudents - enrolledStudents.size() << endl;
}
void Course::addStudent(Student *newStudent)
{
	if (!isFull())
	{
		enrolledStudents.push_back(newStudent);
	}
	else
    {
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
	/*for (int i = 0; i<enrolledStudents.size(); i++)
	{
		if (enrolledStudents[i]->getId() == studentId)
		{
			enrolledStudents.erase(enrolledStudents.begin() + i);
			break;
		}
	}*/
    auto it = std::find_if(
        enrolledStudents.begin(),
        enrolledStudents.end(),
        [studentId](Student* s)
        {
            return s->getId() == studentId;
        });

    if (it != enrolledStudents.end())
    {
        enrolledStudents.erase(it);
    }
}
bool Course::isFull() const
{
	return enrolledStudents.size() >= maxStudents;
}
int  Course::getCourseCode() const
{
	return courseCode;
}
string  Course::getCourseName() const
{
	return courseName;
}
