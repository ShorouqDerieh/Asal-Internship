#include<string>
#include<vector>
//#include "Student.h"
class Student;
using namespace std;
class Course {
private:
	int courseCode;
	string courseName;
	int creditHours;
	int maxStudents;
	vector<Student*> enrolledStudents;
	
public:
	Course(int code, string name, int hours, int maxStudents);
	void displayCourseInfo();
	void addStudent(Student *newStudent);
	void removeStudent(int studentId);
	bool isFull();
	int getCourseCode();
	string getCourseName();

};