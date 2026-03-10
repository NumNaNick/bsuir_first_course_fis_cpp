#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Structure to store student data
struct Student {
    string fullName;
    string groupNumber;
    int physicsGrade;
    int mathGrade;
    int informaticsGrade;
    double averageGrade;
};

// Function to calculate average grade
double calculateAverage(int physics, int math, int informatics) {
    return (physics + math + informatics) / 3.0;
}

// Function to input student data
Student inputStudent() {
    Student student;

    cout << "Enter full name: ";
    cin.ignore();
    getline(cin, student.fullName);

    cout << "Enter group number: ";
    getline(cin, student.groupNumber);

    cout << "Enter physics grade: ";
    cin >> student.physicsGrade;

    cout << "Enter math grade: ";
    cin >> student.mathGrade;

    cout << "Enter informatics grade: ";
    cin >> student.informaticsGrade;

    student.averageGrade = calculateAverage(student.physicsGrade,
        student.mathGrade,
        student.informaticsGrade);

    return student;
}

// Function to display student information
void displayStudent(const Student& student) {
    cout << left << setw(25) << student.fullName
        << setw(15) << student.groupNumber
        << setw(10) << student.physicsGrade
        << setw(12) << student.mathGrade
        << setw(12) << student.informaticsGrade
        << setw(10) << fixed << setprecision(2) << student.averageGrade << endl;
}

// Function to save data to file
void saveToFile(const vector<Student>& students, const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    size_t size = students.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));

    for (const auto& student : students) {
        size_t nameSize = student.fullName.size();
        file.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        file.write(student.fullName.c_str(), nameSize);

        size_t groupSize = student.groupNumber.size();
        file.write(reinterpret_cast<char*>(&groupSize), sizeof(groupSize));
        file.write(student.groupNumber.c_str(), groupSize);

        file.write(reinterpret_cast<const char*>(&student.physicsGrade), sizeof(student.physicsGrade));
        file.write(reinterpret_cast<const char*>(&student.mathGrade), sizeof(student.mathGrade));
        file.write(reinterpret_cast<const char*>(&student.informaticsGrade), sizeof(student.informaticsGrade));
        file.write(reinterpret_cast<const char*>(&student.averageGrade), sizeof(student.averageGrade));
    }

    file.close();
    cout << "Data saved to file: " << filename << endl;
}

// Function to load data from file
vector<Student> loadFromFile(const string& filename) {
    vector<Student> students;
    ifstream file(filename, ios::binary);

    if (!file) {
        cout << "File not found or error opening file!" << endl;
        return students;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));

    for (size_t i = 0; i < size; ++i) {
        Student student;

        size_t nameSize;
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        student.fullName.resize(nameSize);
        file.read(&student.fullName[0], nameSize);

        size_t groupSize;
        file.read(reinterpret_cast<char*>(&groupSize), sizeof(groupSize));
        student.groupNumber.resize(groupSize);
        file.read(&student.groupNumber[0], groupSize);

        file.read(reinterpret_cast<char*>(&student.physicsGrade), sizeof(student.physicsGrade));
        file.read(reinterpret_cast<char*>(&student.mathGrade), sizeof(student.mathGrade));
        file.read(reinterpret_cast<char*>(&student.informaticsGrade), sizeof(student.informaticsGrade));
        file.read(reinterpret_cast<char*>(&student.averageGrade), sizeof(student.averageGrade));

        students.push_back(student);
    }

    file.close();
    cout << "Data loaded from file: " << filename << endl;
    return students;
}

// Function to view all students
void viewStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No data to display!" << endl;
        return;
    }

    cout << "\n" << string(90, '-') << endl;
    cout << left << setw(25) << "Full Name"
        << setw(15) << "Group"
        << setw(10) << "Physics"
        << setw(12) << "Mathematics"
        << setw(12) << "Informatics"
        << setw(10) << "Average" << endl;
    cout << string(90, '-') << endl;

    for (const auto& student : students) {
        displayStudent(student);
    }
    cout << string(90, '-') << endl;
}

// Function to add new student
void addStudent(vector<Student>& students) {
    Student student = inputStudent();
    students.push_back(student);
    cout << "Student added!" << endl;
}

// Function to delete student
void deleteStudent(vector<Student>& students) {
    if (students.empty()) {
        cout << "No data to delete!" << endl;
        return;
    }

    viewStudents(students);
    cout << "Enter student number to delete (1-" << students.size() << "): ";
    int index;
    cin >> index;

    if (index >= 1 && index <= students.size()) {
        students.erase(students.begin() + index - 1);
        cout << "Student deleted!" << endl;
    }
    else {
        cout << "Invalid number!" << endl;
    }
}

// Function to edit student
void editStudent(vector<Student>& students) {
    if (students.empty()) {
        cout << "No data to edit!" << endl;
        return;
    }

    viewStudents(students);
    cout << "Enter student number to edit (1-" << students.size() << "): ";
    int index;
    cin >> index;

    if (index >= 1 && index <= students.size()) {
        cout << "Editing student:" << endl;
        students[index - 1] = inputStudent();
        cout << "Student data updated!" << endl;
    }
    else {
        cout << "Invalid number!" << endl;
    }
}

// Function for individual task solution
void solveIndividualTask(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No data to process!" << endl;
        return;
    }

    // Individual task: find students with average grade higher than 4.0
    vector<Student> excellentStudents;

    for (const auto& student : students) {
        if (student.averageGrade > 4.0) {
            excellentStudents.push_back(student);
        }
    }

    cout << "\nStudents with average grade higher than 4.0:" << endl;
    viewStudents(excellentStudents);

    // Write results to text file
    ofstream textFile("results.txt");
    if (!textFile) {
        cout << "Error creating text file!" << endl;
        return;
    }

    textFile << "All students:\n";
    textFile << string(90, '-') << "\n";
    textFile << left << setw(25) << "Full Name"
        << setw(15) << "Group"
        << setw(10) << "Physics"
        << setw(12) << "Mathematics"
        << setw(12) << "Informatics"
        << setw(10) << "Average" << "\n";
    textFile << string(90, '-') << "\n";

    for (const auto& student : students) {
        textFile << left << setw(25) << student.fullName
            << setw(15) << student.groupNumber
            << setw(10) << student.physicsGrade
            << setw(12) << student.mathGrade
            << setw(12) << student.informaticsGrade
            << setw(10) << fixed << setprecision(2) << student.averageGrade << "\n";
    }

    textFile << "\n\nStudents with average grade higher than 4.0:\n";
    textFile << string(90, '-') << "\n";
    textFile << left << setw(25) << "Full Name"
        << setw(15) << "Group"
        << setw(10) << "Physics"
        << setw(12) << "Mathematics"
        << setw(12) << "Informatics"
        << setw(10) << "Average" << "\n";
    textFile << string(90, '-') << "\n";

    for (const auto& student : excellentStudents) {
        textFile << left << setw(25) << student.fullName
            << setw(15) << student.groupNumber
            << setw(10) << student.physicsGrade
            << setw(12) << student.mathGrade
            << setw(12) << student.informaticsGrade
            << setw(10) << fixed << setprecision(2) << student.averageGrade << "\n";
    }

    textFile.close();
    cout << "Results written to file 'results.txt'" << endl;
}

// Sorting comparison functions
bool compareByName(const Student& a, const Student& b) {
    return a.fullName < b.fullName;
}

bool compareByGroup(const Student& a, const Student& b) {
    return a.groupNumber < b.groupNumber;
}

bool compareByAverageAsc(const Student& a, const Student& b) {
    return a.averageGrade < b.averageGrade;
}

bool compareByAverageDesc(const Student& a, const Student& b) {
    return a.averageGrade > b.averageGrade;
}

// Function to sort students
void sortStudents(vector<Student>& students) {
    if (students.empty()) {
        cout << "No data to sort!" << endl;
        return;
    }

    cout << "\nChoose sorting type:\n";
    cout << "1. By name (alphabetical)\n";
    cout << "2. By group number\n";
    cout << "3. By average grade (ascending)\n";
    cout << "4. By average grade (descending)\n";
    cout << "Your choice: ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
        sort(students.begin(), students.end(), compareByName);
        cout << "Sorting by name completed!" << endl;
        break;
    case 2:
        sort(students.begin(), students.end(), compareByGroup);
        cout << "Sorting by group completed!" << endl;
        break;
    case 3:
        sort(students.begin(), students.end(), compareByAverageAsc);
        cout << "Sorting by average grade (ascending) completed!" << endl;
        break;
    case 4:
        sort(students.begin(), students.end(), compareByAverageDesc);
        cout << "Sorting by average grade (descending) completed!" << endl;
        break;
    default:
        cout << "Invalid choice!" << endl;
        return;
    }

    viewStudents(students);
}

// Main menu
void displayMenu() {
    cout << "\n=== MENU ===" << endl;
    cout << "1. Create/Load file" << endl;
    cout << "2. View all students" << endl;
    cout << "3. Add student" << endl;
    cout << "4. Solve individual task" << endl;
    cout << "5. Delete student" << endl;
    cout << "6. Edit student" << endl;
    cout << "7. Sort students" << endl;
    cout << "8. Save data" << endl;
    cout << "0. Exit" << endl;
    cout << "Choose menu option: ";
}

int main() {
    vector<Student> students;
    string filename = "students.dat";
    bool dataChanged = false;

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            students = loadFromFile(filename);
            if (students.empty()) {
                cout << "Created new file for data storage." << endl;
            }
            break;
        case 2:
            viewStudents(students);
            break;
        case 3:
            addStudent(students);
            dataChanged = true;
            break;
        case 4:
            solveIndividualTask(students);
            break;
        case 5:
            deleteStudent(students);
            dataChanged = true;
            break;
        case 6:
            editStudent(students);
            dataChanged = true;
            break;
        case 7:
            sortStudents(students);
            dataChanged = true;
            break;
        case 8:
            saveToFile(students, filename);
            dataChanged = false;
            break;
        case 0:
            if (dataChanged) {
                cout << "There are unsaved changes. Save? (y/n): ";
                char saveChoice;
                cin >> saveChoice;
                if (saveChoice == 'y' || saveChoice == 'Y') {
                    saveToFile(students, filename);
                }
            }
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}