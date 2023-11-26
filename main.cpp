#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

const string STUDENTS_FILENAME = "./data/students.txt";
const string COURSES_FILENAME = "./data/courses.txt";
const string REGISTERS_FILENAME = "./data/registers.txt";

const int ERROR_THREASHOLD = 2;

const int MAX_UNITS_LOW = 15;
const int MAX_UNITS_NORMAL = 20;
const int MAX_UNITS_HIGH = 24;

struct StudentNode{
    unsigned int id;
    string fname;
    string lname;
    float average;
    unsigned int unitsCount = 0;
    StudentNode* next = NULL;
};

struct CourseNode{
    unsigned int code;
    string title;
    int units;
    int capacity;
    int studentCount = 0;
    CourseNode* next = NULL;
};

struct RegisterNode{
    StudentNode* student;
    CourseNode* course;
    RegisterNode* next  = NULL;
};

void changeFileMode(fstream&, string, int);
void checkDatabase(fstream&, string);
void printLine();
string lowerCase(string);
bool compareString(string, string);
string int2String(int);
int menu();
int listLength(StudentNode*);
StudentNode* searchStudent(StudentNode*, unsigned int);
CourseNode* searchCourse(CourseNode*, unsigned int);
RegisterNode* searchRegister(RegisterNode*, StudentNode*, CourseNode*);
StudentNode* addStudent(StudentNode*, unsigned int, string, string, float);
CourseNode* addCourse(CourseNode*, unsigned int, string, int, int);
RegisterNode* addRegister(RegisterNode*, StudentNode*, CourseNode*);
StudentNode* readStudents(fstream&, string, StudentNode*);
CourseNode* readCourses(fstream &, string, CourseNode*);
RegisterNode* readRegisters(fstream &, string, RegisterNode*, StudentNode*, CourseNode*);
RegisterNode* deleteAllRegisterByStudent(RegisterNode*, StudentNode*);
RegisterNode* deleteAllRegisterByCourse(RegisterNode*, CourseNode*);
RegisterNode* deleteRegister(RegisterNode*, StudentNode*, CourseNode*);
StudentNode* deleteStudent(StudentNode*, StudentNode* student);
CourseNode* deleteCourse(CourseNode*, CourseNode*);
void writeStudents(fstream&, string, StudentNode*);
void writeCourses(fstream &, string, CourseNode*);
void writeRegisters(fstream &, string, RegisterNode*);
void printCourse(CourseNode*);
void printStudent(StudentNode*);
StudentNode* selectStudent(StudentNode*, string, int);
int showStudentQuery(StudentNode*, string);
StudentNode* queryStudent(StudentNode*);
CourseNode* selectCourse(CourseNode*, string, int);
int showCourseQuery(CourseNode*, string);
CourseNode* queryCourse(CourseNode*);
void getRegistersByStudent(RegisterNode*, StudentNode*);
void getRegistersByCourse(RegisterNode*, CourseNode*);
void getStudentData(StudentNode*, RegisterNode*);
void getCourseData(CourseNode*, RegisterNode*);
bool compareStudent(StudentNode*, StudentNode*, int);
StudentNode* sortStudentList(StudentNode*, int);
StudentNode* newStudent(StudentNode*);
CourseNode* newCourse(CourseNode*);
RegisterNode* newRegister(RegisterNode*, StudentNode*, CourseNode*);
RegisterNode* removeRegister(RegisterNode*, StudentNode*, CourseNode*);
void printStudentList(StudentNode*);
void printCourseList(CourseNode*);
int studentListSortMethod();

int main(){

    fstream studentsFile, coursesFile, registersFile;
    checkDatabase(studentsFile, STUDENTS_FILENAME);
    checkDatabase(coursesFile, COURSES_FILENAME);
    checkDatabase(registersFile, REGISTERS_FILENAME);

    StudentNode* studentsList = NULL;
    CourseNode* coursesList = NULL;
    RegisterNode* registersList = NULL;

    studentsList = readStudents(studentsFile, STUDENTS_FILENAME, studentsList);
    coursesList = readCourses(coursesFile, COURSES_FILENAME, coursesList);
    registersList = readRegisters(registersFile, REGISTERS_FILENAME, registersList, studentsList, coursesList);

    while (true){
        int choice = menu();
        switch (choice){
            case 1:
                coursesList = newCourse(coursesList);
                writeCourses(coursesFile, COURSES_FILENAME, coursesList);
                cout << "DONE!" << endl;
                break;
            case 2:
                studentsList = newStudent(studentsList);
                writeStudents(studentsFile, STUDENTS_FILENAME, studentsList);
                cout << "DONE!" << endl;
                break;
            case 3:
                registersList = newRegister(registersList, studentsList, coursesList);
                writeRegisters(registersFile, REGISTERS_FILENAME, registersList);
                cout << "DONE!" << endl;
                break;
            case 4:
                printCourseList(coursesList);
                break;
            case 5: {
                int choice = studentListSortMethod();
                studentsList = sortStudentList(studentsList, choice);
                printStudentList(studentsList);
                break;
            }
            case 6:
                getCourseData(coursesList, registersList);
                break;
            case 7:
                getStudentData(studentsList, registersList);
                break;
            case 8: {
                CourseNode* course = queryCourse(coursesList);
                registersList = deleteAllRegisterByCourse(registersList, course);
                writeRegisters(registersFile, REGISTERS_FILENAME, registersList);
                coursesList = deleteCourse(coursesList, course);
                writeCourses(coursesFile, COURSES_FILENAME, coursesList);
                cout << "DONE!" << endl;
                break;
            }
            case 9: {
                StudentNode* student = queryStudent(studentsList);
                registersList = deleteAllRegisterByStudent(registersList, student);
                writeRegisters(registersFile, REGISTERS_FILENAME, registersList);
                studentsList = deleteStudent(studentsList, student);
                writeStudents(studentsFile, STUDENTS_FILENAME, studentsList);
                cout << "DONE!" << endl;
                break;
            }
            case 10:
                registersList = removeRegister(registersList, studentsList, coursesList);
                writeRegisters(registersFile, REGISTERS_FILENAME, registersList);
                cout << "DONE!" << endl;
                break;
            case 11:
                exit(0);
                break;
            default:
                break;
        }
    }
}

void changeFileMode(fstream &file, string fileName, int mode){
    switch (mode)
    {
    case 1:
        file.open(fileName, ios::in);
        break;
    case 2:
        file.open(fileName, ios::out);
        break;
    case 3:
        file.open(fileName, ios::app);
        break;
    case 4: 
        file.close();
        break;
    default:
        break;
    }
}

void checkDatabase(fstream &file, string fileName){
    changeFileMode(file, fileName, 1);
    if (! file){
        cout << "Can not read file" << fileName << "! Program terminated!" << endl;
        exit(1);
    }
}

void printLine(){
    for (int i = 0; i < 25; i++){
        cout << "-";
    }
    cout << endl;
}

int menu(){
    int choice;
    do {
        cout << "1- Add course\n2- Add student\n3- Add register\n4- List Courses\n5- List Students\n6- Lookup Course\n7- Lookup Student\n8- Remove course\n9- Remove student\n10- Remove register\n11- exit\noption?...\t";
        cin >> choice;
        if (choice > 11 || choice < 1){
            cout << "Invalid option!" << endl;
        }
    } while (choice > 11 || choice < 1);
    return choice;
}

int listLength(StudentNode* list){
    StudentNode* current = list;
    int counter = 0;
    while (current){
        counter++;
        current = current->next;
    }
    return counter;
}

string lowerCase(string str){
    string result = str;
    for (int i = 0; i < result.length(); i++){
        str[i] = tolower(result[i]);
    }
    return result;
}

string int2String(int input){
    stringstream toBeConverted;
    toBeConverted << input;
    return toBeConverted.str();
}

bool compareString(string inp1, string inp2){
    string str1 = lowerCase(inp1), str2 = lowerCase(inp2);
    int faultCount = 0;
    if (str1.length() == str2.length()){
        for (int i = 0; i < str1.length(); i++){
            if (str1[i] != str2[i]){
                faultCount++;
            }
        }
    } else {
        string longStr = (str1.length() > str2.length())? str1: str2;
        string shortStr = (longStr == str1)? str2: str1;
        if (longStr.length() - shortStr.length() <= ERROR_THREASHOLD){
            for (int i = 0, j = 0; i < longStr.length() && j < shortStr.length(); i++, j++){
                char c1 = longStr[i], c2 = shortStr[j];
                if (shortStr[j] != longStr[i]){
                    faultCount++;
                    j--;
                }
            }

        } else {
            return false;
        }
    }
    if (faultCount <= ERROR_THREASHOLD){
        return true;
    }
    return false;
}

StudentNode* searchStudent(StudentNode* head, unsigned int id){
    StudentNode* current = head;
    while (current){
        if (current->id == id){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

CourseNode* searchCourse(CourseNode* head, unsigned int code){
    CourseNode* current = head;
    while (current){
        if (current->code == code){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

RegisterNode* searchRegister(RegisterNode* head, StudentNode* student, CourseNode* course){
    RegisterNode* current = head;
    while (current){
        if (current->student == student && current->course == course){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

StudentNode* addStudent(StudentNode* head, unsigned int id, string name, string lastname, float average){
    StudentNode* node = new StudentNode;
    node->id = id;
    node->fname = name;
    node->lname = lastname;
    node->average = average;
    if (head == NULL){
        node->next = head;
        return node;
    }
    if (lastname < head->lname){
        node->next = head;
        return node;
    }
    StudentNode* p = head;
    StudentNode* q = head->next;
    while (q && q->lname < lastname){
        p = q;
        q = q->next;
    }
    p->next = node;
    node->next = q;
    return head;
}

CourseNode* addCourse(CourseNode* head, unsigned int code, string title, int units, int capacity){
    CourseNode* node = new CourseNode;
    node->code = code;
    node->title = title;
    node->units = units;
    node->capacity = capacity;
    if (head == NULL){
        node->next = head;
        return node;
    }
    if (code < head->code){
        node->next = head;
        return node;
    }
    CourseNode* p = head;
    CourseNode* q = head->next;
    while (q && q->code < code){
        p = q;
        q = q->next;
    }
    p->next = node;
    node->next = q;
    return head;
}

RegisterNode* addRegister(RegisterNode* head, StudentNode* student, CourseNode* course){
    if (student->average < 12.0 && (student->unitsCount + course->units) > MAX_UNITS_LOW){
        cout << "Student has reached their maximum units." << endl;
        return head;
    } else if (student->average > 12.0 && student->average < 18.0 && (student->unitsCount + course->units) > MAX_UNITS_NORMAL){
        cout << "Student has reached their maximum units." << endl;
        return head;
    } else if (student->average > 18.0 && (student->unitsCount + course->units) > MAX_UNITS_HIGH){
        cout << "Student has reached their maximum units." << endl;
        return head;      
    }
    if (course->studentCount + 1 > course->capacity){
        cout << "Course has reached its maximum capacity." << endl;
        return head;
    }
    RegisterNode* node = new RegisterNode;
    node->student = student;
    student->unitsCount += course->units;
    course->studentCount++;
    node->course = course;
    if (head == NULL){
        node->next = head;
        return node;
    }
    if (student->lname < head->student->lname){
        node->next = head;
        return node;
    }
    RegisterNode* p = head;
    RegisterNode* q = head->next;
    while (q && q->student->lname < student->lname){
        p = q;
        q = q->next;
    }
    p->next = node;
    node->next = q;
    return head;
}

StudentNode* readStudents(fstream &file, string fileName ,StudentNode* list){
    string id, name, lastname, average;
    while (getline(file, id, ';')){
        if (id == "END-OF-FILE"){
            break;
        } 
        getline(file, name, ';');
        getline(file, lastname, ';');
        getline(file, average, '\n');
        list = addStudent(list, std::stoi(id), name, lastname, std::stof(average));
    }
    changeFileMode(file, fileName, 4);
    return  list;
}

CourseNode* readCourses(fstream &file, string fileName, CourseNode* list){
    string code, title, units, capacity;
    while (getline(file, code, ';')){
        if (code == "END-OF-FILE"){
            break;
        } 
        getline(file, title, ';');
        getline(file, units, ';');
        getline(file, capacity, '\n');
        list = addCourse(list, std::stoi(code), title, std::stoi(units), std::stoi(capacity));
    }
    changeFileMode(file, fileName, 4);
    return list;
}

RegisterNode* readRegisters(fstream &file, string fileName, RegisterNode* registersList, StudentNode* studentsList, CourseNode* coursesList){
    string id, code;
    while (getline(file, id, ';')){
        if (id == "END-OF-FILE"){
            break;
        } 
        getline(file, code, '\n');
        CourseNode* course = searchCourse(coursesList, std::stoi(code));
        StudentNode* student = searchStudent(studentsList, std::stoi(id));
        if (course && student){
            registersList = addRegister(registersList, student, course);
        }
    }
    changeFileMode(file, fileName, 4);
    return registersList;
}

RegisterNode* deleteAllRegisterByStudent(RegisterNode* head, StudentNode* student){
    while (head && head->student == student){
        RegisterNode* temp = head;
        head = head->next;
        temp->student->unitsCount -= temp->course->units;
        temp->course->studentCount--;
        delete temp;
    }
    if (head == NULL){
        return NULL;
    }
    RegisterNode* p = head;
    RegisterNode* q = head->next;
    while (q){
        if (q->student == student){
            RegisterNode* temp = q;
            q = q->next;
            p->next = q;
            temp->student->unitsCount -= temp->course->units;
            temp->course->studentCount--;
            delete temp;
        } else {
            p = q;
            q = q->next;
        }
    }
    return head;
}

RegisterNode* deleteAllRegisterByCourse(RegisterNode* head, CourseNode* course){
    while (head && head->course == course){
        RegisterNode* temp = head;
        head = head->next;
        temp->student->unitsCount -= temp->course->units;
        temp->course->studentCount--;
        delete temp;
    }
    if (head == NULL){
        return NULL;
    }
    RegisterNode* p = head;
    RegisterNode* q = head->next;
    while (q){
        if (q->course == course){
            RegisterNode* temp = q;
            q = q->next;
            p->next = q;
            temp->student->unitsCount -= temp->course->units;
            temp->course->studentCount--;
            delete temp;
        } else {
            p = q;
            q = q->next;
        }
    }
    return head;
}

RegisterNode* deleteRegister(RegisterNode* head, StudentNode* student, CourseNode* course){
    while (head && head->course == course && head->student == student){
        RegisterNode* temp = head;
        head = head->next;
        temp->student->unitsCount -= temp->course->units;
        temp->course->studentCount--;
        delete temp;
    }
    if (head == NULL){
        return NULL;
    }
    RegisterNode* p = head;
    RegisterNode* q = head->next;
    while (q){
        if (q->course == course && q->student == student){
            RegisterNode* temp = q;
            q = q->next;
            p->next = q;
            temp->student->unitsCount -= temp->course->units;
            temp->course->studentCount--;
            delete temp;
        } else {
            p = q;
            q = q->next;
        }
    }
    return head;
}

StudentNode* deleteStudent(StudentNode* head, StudentNode* student){
    while (head && head == student){
        StudentNode* temp = head;
        head = head->next;
        delete temp;
    }
    if (head == NULL){
        return NULL;
    }
    StudentNode* p = head;
    StudentNode* q = head->next;
    while (q){
        if (q == student){
            StudentNode* temp = q;
            q = q->next;
            p->next = q;
            delete temp;
        } else {
            p = q;
            q = q->next;
        }
    }
    return head;
}

CourseNode* deleteCourse(CourseNode* head, CourseNode* course){
    while (head && head == course){
        CourseNode* temp = head;
        head = head->next;
        delete temp;
    }
    if (head == NULL){
        return NULL;
    }
    CourseNode* p = head;
    CourseNode* q = head->next;
    while (q){
        if (q == course){
            CourseNode* temp = q;
            q = q->next;
            p->next = q;
            delete temp;
        } else {
            p = q;
            q = q->next;
        }
    }
    return head;
}

void writeStudents(fstream &file, string fileName, StudentNode* list){
    StudentNode* current = list;
    changeFileMode(file, fileName, 2);
    while (current){
        file << current->id << ";" << current->fname << ";" << current->lname << ";" << current->average << "\n";
        current = current->next;
    }
    file << "END-OF-FILE;";
    changeFileMode(file, fileName, 4);
}

void writeCourses(fstream &file, string fileName, CourseNode* list){
    CourseNode* current = list;
    changeFileMode(file, fileName, 2);
    while (current){
        file << current->code << ";" << current->title << ";" << current->units << ";" << current->capacity << "\n";
        current = current->next;
    }
    file << "END-OF-FILE;";
    changeFileMode(file, fileName, 4);
}

void writeRegisters(fstream &file, string fileName, RegisterNode* list){
    RegisterNode* current = list;
    changeFileMode(file, fileName, 2);
    while (current){
        file << current->student->id << ";" << current->course->code << "\n";
        current = current-> next;
    }
    file << "END-OF-FILE;";
    changeFileMode(file, fileName, 4);
}

void getRegistersByStudent(RegisterNode* list, StudentNode* student){
    RegisterNode* current = list;
    while (current){
        if (current->student == student){
            printCourse(current->course);
        }
        current = current->next;
    }
}

void getRegistersByCourse(RegisterNode* list, CourseNode* course){
    RegisterNode* current = list;
    while (current){
        if (current->course == course){
            printStudent(current->student);
        }
        current = current->next;
    }
}

void getStudentData(StudentNode* studentsList, RegisterNode* registersList){
    string id;
    StudentNode* student;
    student = queryStudent(studentsList);
    printStudent(student);
    printLine();
    cout << "Courses registered by student:" << endl;
    getRegistersByStudent(registersList, student);
}

void getCourseData(CourseNode* coursesList, RegisterNode* registersList){
    string code;
    CourseNode* course;
    course = queryCourse(coursesList);
    printCourse(course);
    printLine();
    cout << "Students registered in course:" << endl;
    getRegistersByCourse(registersList, course);
}

bool compareStudent(StudentNode* a, StudentNode* b, int mode){
    switch(mode){
        case 1:
            if (a->lname >= b->lname){
                return true;
            }
            break;
        case 2:
            if (a->average >= b->average){
                return true;
            }
            break;
        case 3:
            if (a->unitsCount >= b->unitsCount){
                return true;
            }
            break;
        default:
            return false;
            break;
    }
    return false;
}

StudentNode* sortStudentList(StudentNode* list, int mode){
    int n = listLength(list) - 1;
    while(n){
        StudentNode* prev = NULL;
        StudentNode* current = list;
        while(current->next){
            if(compareStudent(current, current->next, mode)){
                if(prev == NULL){
                    StudentNode* temp = current->next;
                    current->next = temp->next;
                    temp->next = current;
                    prev = temp;
                    list = prev;
                } else {
                    StudentNode* temp = current->next;
                    prev->next = temp;
                    current->next = temp->next;
                    temp->next = current;
                    prev = temp;
                }
            } else {
                prev = current; 
                current=current->next;
            }
        }
        n--;
    }
    return list ;
}

StudentNode* newStudent(StudentNode* list){
    string id, fname, lname, average;
    do {
        cout << "Student ID:\t";
        cin >> id;
        cin.ignore();
        if (searchStudent(list, std::stoi(id))){
            cout << "Student with ID " << id << " already exists!" << endl;
        }
    } while (searchStudent(list, std::stoi(id)));
    cout << "First Name:\t";
    getline(cin, fname, '\n');
    cout << "Last Name:\t";
    getline(cin, lname, '\n');
    cout << "Average:\t";
    cin >> average;
    list = addStudent(list, std::stoi(id), fname, lname, std::stof(average));
    return list;
}

CourseNode* newCourse(CourseNode* list){
    string code, title, unit, capacity;
    do {
        cout << "Course Code:\t";
        cin >> code;
        cin.ignore();
        if (searchCourse(list, std::stoi(code))){
            cout << "Course with code " << code << " already exists!" << endl;
        }
    } while (searchCourse(list, std::stoi(code)));
    cout << "Title:\t";
    getline(cin, title, '\n');
    cout << "Units:\t";
    cin >> unit;
    cout << "Capacity:\t";
    cin >> capacity;
    list = addCourse(list, std::stoi(code), title, std::stoi(unit), std::stoi(capacity));
    return list;
}

RegisterNode* newRegister(RegisterNode* registersList, StudentNode* studentsList, CourseNode* coursesList){
    string code, id;
    StudentNode* student;
    CourseNode* course;
    student = queryStudent(studentsList);
    course = queryCourse(coursesList);
    if (searchRegister(registersList, student, course)){
        cout << "Register already exists!" << endl;
        return registersList;
    }
    registersList = addRegister(registersList, student, course);
    return registersList;
}

RegisterNode* removeRegister(RegisterNode* registersList, StudentNode* studentsList, CourseNode* coursesList){
    string code, id;
    StudentNode* student;
    CourseNode* course;
    student = queryStudent(studentsList);
    course = queryCourse(coursesList);
    if (! searchRegister(registersList, student, course)){
        cout << "Register doesn't exists!" << endl;
        return registersList;
    }
    registersList = deleteRegister(registersList, student, course);
    return registersList;
}

void printStudent(StudentNode* student){
    cout << "ID: " << student->id << " | Name: " << student->fname << " | Last Name: " << student->lname << " | Average Score: " << student->average << " | Number of Units: " << student->unitsCount << endl;
}

void printCourse(CourseNode* course){
    cout << "Code: " << course->code << " | Title: " << course->title << " | Units: " << course->units << " | Capacity: " << course->capacity << " | Registered students: " << course->studentCount << endl;
}

void printStudentList(StudentNode* list){
    StudentNode* current = list;
    while (current){
        printStudent(current);
        current = current->next;
        printLine();
    }
}

void printCourseList(CourseNode* list){
    CourseNode* current = list;
    while (current){
        printCourse(current);
        current = current->next;
        printLine();
    }
}

int studentListSortMethod(){
    int choice;
    do {
        cout << "1- Sort by Name\n2- Sort by Average\n3- Sort by Units\noption?...\t" << endl;
        cin >> choice;
        if (choice > 3 || choice < 1){
            cout << "Invalid option!" << endl;
        }
    } while (choice > 3 || choice < 1);
    return choice;
}

StudentNode* selectStudent(StudentNode* list, string query, int number){
    int counter = 0;
    StudentNode* current = list;
    while(current){
        if (int2String(current->id) == query || compareString(current->fname, query) || compareString(current->lname, query)){
            counter++;
            if (counter == number){
                return current;
            }
        }
        current = current->next;
    }
    return NULL;
}

int showStudentQuery(StudentNode* list, string query){
    StudentNode* current = list;
    int counter = 0;
    while(current){
        if (int2String(current->id) == query || compareString(current->fname, query) || compareString(current->lname, query)){
            counter++;
            cout << counter << " - ";
            printStudent(current);
        }
        current = current->next;
    }
    return counter;
}

StudentNode* queryStudent(StudentNode* list){
    StudentNode* current = list;
    string query;
    int results;
    do{
        cout << "query for student?...\t";
        cin >> query;
        cin.ignore();
        results = showStudentQuery(list, query);
        if (! results){
            cout << "Your query did not have any result!" << endl;
        } else {
            int number;
            do {
                cout << "?...\t";
                cin >> number;
                if (number > results || number < 1){
                    cout << "Invalid input!" << endl;
                }
            } while (number > results || number < 1);
            return selectStudent(list, query, number);
        }
    } while (! results);
    return NULL;
}

CourseNode* selectCourse(CourseNode* list, string query, int number){
    int counter = 0;
    CourseNode* current = list;
    while(current){
        if (int2String(current->code) == query || compareString(current->title, query)){
            counter++;
            if (counter == number){
                return current;
            }
        }
        current = current->next;
    }
    return NULL;
}

int showCourseQuery(CourseNode* list, string query){
    CourseNode* current = list;
    int counter = 0;
    while(current){
        if (int2String(current->code) == query || compareString(current->title, query)){
            counter++;
            cout << counter << " - ";
            printCourse(current);
        }
        current = current->next;
    }
    return counter;
}

CourseNode* queryCourse(CourseNode* list){
    CourseNode* current = list;
    string query;
    int results;
    do{
        cout << "query for course?...\t";
        cin >> query;
        cin.ignore();
        results = showCourseQuery(list, query);
        if (! results){
            cout << "Your query did not have any result!" << endl;
        } else {
            int number;
            do {
                cout << "?...\t";
                cin >> number;
                if (number > results || number < 1){
                    cout << "Invalid input!" << endl;
                }
            } while (number > results || number < 1);
            return selectCourse(list, query, number);
        }
    } while (! results);
    return NULL;
}