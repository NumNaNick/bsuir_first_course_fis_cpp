#include <iostream>
#include <iomanip>
#include <cstring>

//14. Найти информацию о студентах-отличниках интересующей вас группы.

using namespace std;

const int MAX_STUDENTS = 100;
struct Student {
    char surname[32];
    char group[7];
    int m1, m2, m3;
    double avg;
    bool used; // индикатор использования записи
};

Student students[MAX_STUDENTS];
int studentCount = 0;

void initArray() {
    for (int i = 0; i < MAX_STUDENTS; ++i) students[i].used = false;
    studentCount = 0;
}

void printHeader() {
    cout << fixed << setprecision(2);
        cout << setw(4) << "#" << " | "
            << setw(32) << "Surname" << " | "
            << setw(7) << "Group" << " | "
         << setw(4) << "M1" << " " << setw(4) << "M2" << " " << setw(4) << "M3" << " | "
         << setw(6) << "Avg" << endl;
    for (int i = 0; i < 77; ++i) cout << '-';
    cout << endl;
}

void viewAll() {
    printHeader();
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        if (!students[i].used) continue;
           cout << setw(4) << i << " | "
               << setw(32) << students[i].surname << " | "
               << setw(7) << students[i].group << " | "
             << setw(4) << students[i].m1 << " " << setw(4) << students[i].m2 << " " << setw(4) << students[i].m3 << " | "
             << setw(6) << students[i].avg << endl;
    }
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "Array full. Cannot add more students.\n";
        return;
    }
    int idx = -1;
    for (int i = 0; i < MAX_STUDENTS; ++i) if (!students[i].used) { idx = i; break; }
    if (idx == -1) return;
    cout << "Enter surname and group: ";
    cin >> setw(32) >> students[idx].surname >> setw(7) >> students[idx].group;
    if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid input for surname/group. Aborting add.\n"; return; }
    cout << "Enter three grades (integers): ";
    while (!(cin >> students[idx].m1 >> students[idx].m2 >> students[idx].m3)) {
        cout << "Invalid grades. Enter three integers: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    students[idx].avg = (students[idx].m1 + students[idx].m2 + students[idx].m3) / 3.0;
    students[idx].used = true;
    ++studentCount;
    cout << "Student added at index " << idx << ".\n";
}

void editStudent() {
    cout << "Enter index to edit: ";
    int idx;
    if (!(cin >> idx) || idx < 0 || idx >= MAX_STUDENTS || !students[idx].used) {
        cout << "Invalid index.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    cout << "Current: " << students[idx].surname << " " << students[idx].group << " "
         << students[idx].m1 << " " << students[idx].m2 << " " << students[idx].m3 << " Avg=" << students[idx].avg << "\n";
    cout << "Enter new surname and group: "; cin >> setw(32) >> students[idx].surname >> setw(7) >> students[idx].group;
    if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid input. Edit aborted.\n"; return; }
    cout << "Enter three grades: ";
    while (!(cin >> students[idx].m1 >> students[idx].m2 >> students[idx].m3)) {
        cout << "Invalid grades. Enter three integers: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    students[idx].avg = (students[idx].m1 + students[idx].m2 + students[idx].m3) / 3.0;
    cout << "Updated.\n";
}

void deleteStudent() {
    cout << "Enter index to delete: ";
    int idx;
    if (!(cin >> idx) || idx < 0 || idx >= MAX_STUDENTS || !students[idx].used) {
        cout << "Invalid index.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    students[idx].used = false;
    --studentCount;
    cout << "Deleted record at index " << idx << ".\n";
}

// сортировка по среднему баллу по убыванию
void sortByAvgDesc() {
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        for (int j = 0; j + 1 < MAX_STUDENTS; ++j) {
            if (!students[j].used || !students[j+1].used) continue;
            if (students[j].avg < students[j+1].avg) {
                Student tmp = students[j];
                students[j] = students[j+1];
                students[j+1] = tmp;
            }
        }
    }
    cout << "Sorted by average (descending).\n";
}

// сортировка по среднему баллу по возрастанию
void sortByAvgAsc() {
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        for (int j = 0; j + 1 < MAX_STUDENTS; ++j) {
            if (!students[j].used || !students[j+1].used) continue;
            if (students[j].avg > students[j+1].avg) {
                Student tmp = students[j];
                students[j] = students[j+1];
                students[j+1] = tmp;
            }
        }
    }
    cout << "Sorted by average (ascending).\n";
}

// сортировка по фамилии (алфавит)
void sortBySurname() {
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        for (int j = 0; j + 1 < MAX_STUDENTS; ++j) {
            if (!students[j].used || !students[j+1].used) continue;
            if (strcmp(students[j].surname, students[j+1].surname) > 0) {
                Student tmp = students[j];
                students[j] = students[j+1];
                students[j+1] = tmp;
            }
        }
    }
    cout << "Sorted by surname (alphabetical).\n";
}

// найти отличников в заданной группе
void solveTask() {
    char targetGroup[7];
    cout << "Enter group to find excellent students: "; cin >> setw(7) >> targetGroup; cout << endl;
    bool found = false;
    printHeader();
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        if (!students[i].used) continue;
        if (strcmp(students[i].group, targetGroup) == 0 && students[i].avg >= 9.0) {
            cout << setw(4) << i << " | "
                 << setw(32) << students[i].surname << " | "
                 << setw(7) << students[i].group << " | "
                 << setw(4) << students[i].m1 << " " << setw(4) << students[i].m2 << " " << setw(4) << students[i].m3 << " | "
                 << setw(6) << students[i].avg << endl;
            found = true;
        }
    }
    if (!found) cout << "No excellent students found in group " << targetGroup << ".\n";
}

int main() {
    initArray();
    bool running = true;
    while (running) {
        cout << "\nChoose action:\n"
             << "1 - View all\n"
             << "2 - Add student\n"
             << "3 - Edit student\n"
             << "4 - Delete student\n"
             << "5 - Sort by average (desc)\n"
             << "6 - Sort by average (asc)\n"
             << "7 - Sort by surname (alphabetically)\n"
             << "8 - Find excellent students in group\n"
             << "9 - Clear array\n"
             << "10 - Exit\n"
             << "Choice: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number from the menu.\n";
            continue;
        }
        switch (choice) {
            case 1: viewAll(); break;
            case 2: addStudent(); break;
            case 3: editStudent(); break;
            case 4: deleteStudent(); break;
            case 5: sortByAvgDesc(); break;
            case 6: sortByAvgAsc(); break;
            case 7: sortBySurname(); break;
            case 8: solveTask(); break;
            case 9: initArray(); cout << "Array cleared.\n"; break;
            case 10: running = false; break;
            default: cout << "Invalid choice.\n"; break;
        }
    }
    cout << "You exited the program\n";
    return 0;
}