#include <iostream>
#include<vector>
#include<string>
#include "Person.h"
#include "Student.h"
#include "Course.h"
#include "Professor.h"
using namespace std;
int main()
{
	vector<Person*>persons;
	Student p1 (1, "Alice", "alice@gmail.com", "Computer Science", 3.5);
	Student p2 (2, "Bob", "bob@gmail.com", "Mathematics", 3.7);
	Professor p3(1, "Dr. Smith", "smith@gmail.com", 101, "Mathematics");
	persons.push_back(&p1);
	persons.push_back(&p2);
	persons.push_back(&p3);
	vector<Course*>courses = {
		new Course(101,"Mathematics",3,30),
		new Course(102,"Differential Equations",4,1),
		new Course(103,"Integral Calculus",3,20)
	};
	p3.addCourse(courses[0]);
	p3.addCourse(courses[1]);
	p3.addCourse(courses[2]);
	p1.registerCourse(courses[0]);
	p2.registerCourse(courses[1]);
	cout << "================================" << endl << "COURSES INFORMATION" << endl<<endl;
	for (Course* course : courses)
	{
		course->displayCourseInfo();
		cout <<  endl;
	}
	cout << "================================" << endl << "STUDENT INFORMATION" << endl<<endl;
	for (Person* person : persons)
	{
		if (Student* student = dynamic_cast<Student*>(person))
		{
			student->displayInfo();
			student->displayRegisteredCourses();
			cout << endl;
		}
	}
	cout << "================================" << endl << "PROFESSOR INFORMATION" << endl<<endl;
	for (Person* person : persons)
	{
		if (Professor* professor = dynamic_cast<Professor*>(person))
		{
			professor->displayInfo();
			professor->displayCourses();
			cout  << endl;
		}
	}
	cout << "================================" << endl << "MESSAGES" << endl<<endl;
	p1.registerCourse(courses[0]);
	p1.registerCourse(courses[1]);
	p2.dropCourse(102);
    for (Course* course : courses)
    {
        delete course;
    }
}
