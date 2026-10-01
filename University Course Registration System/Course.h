#include<string>
#include<vector>
class Student;
using namespace std;
class Course
{
private:
	int courseCode;
	string courseName;
	int creditHours;
	int maxStudents;
	vector<Student*> enrolledStudents;
	
public:
	Course(int code, string name, int hours, int maxStudents);
	void displayCourseInfo() const;
	void addStudent(Student *newStudent);
	void removeStudent(int studentId);
	bool isFull()const;
	int getCourseCode()const;
	string getCourseName()const;

};
