#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct Student {
    string surname;
    string group;
    int m1;
    int m2;
    int m3;
    double avg;
};

void CreateFile();
void ViewFile();
void AppendToFile();
void EditFile();
void DeleteFromFile();
void SortFile();
void SolveTask();

bool readStudentsFromFile(const string &fileName, vector<Student> &students) {
    students.clear();
    ifstream in(fileName);
    if (!in) return false;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        Student s; 
        if (!(ss >> s.surname >> s.group >> s.m1 >> s.m2 >> s.m3)) continue;
        if (!(ss >> s.avg)) {
            s.avg = (s.m1 + s.m2 + s.m3) / 3.0;
        }
        students.push_back(s);
    }
    return true;
}

bool writeStudentsToFile(const string &fileName, const vector<Student> &students) {
    ofstream out(fileName);
    if (!out) return false;
    for (const auto &s : students) {
        out << s.surname << " " << s.group << " " << s.m1 << " " << s.m2 << " " << s.m3 << " ";
        out << fixed << setprecision(2) << s.avg << "\n";
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "RU");
    bool run = true;
    while (run) {
        cout << "Выберите действие:" << endl
             << "1. Создание" << endl
             << "2. Просмотр" << endl
             << "3. Добавление" << endl
             << "4. Решение индивидуального задания" << endl
             << "5. Редактирование" << endl
             << "6. Удаление" << endl
             << "7. Сортировка" << endl
             << "8. Выход из программы" << endl
             << "Ввод данных:";
        int action;
        if (!(cin >> action)) break;
        switch(action) {
            case 1: CreateFile(); break;
            case 2: ViewFile(); break;
            case 3: AppendToFile(); break;
            case 4: SolveTask(); break;
            case 5: EditFile(); break;
            case 6: DeleteFromFile(); break;
            case 7: SortFile(); break;
            case 8: run = false; break;
            default: cout << "Неверный выбор." << endl; break;
        }
    }
    return 0;
}

void CreateFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    ofstream out(fileName);
    if (!out) {
        cout << "Ошибка при создании файла." << endl;
        return;
    }
    cout << "Файл создан успешно." << endl;
}

void ViewFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    vector<Student> students;
    if (!readStudentsFromFile(fileName, students)) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    cout << "\nСодержимое файла:" << endl;
    cout << "==================" << endl;
    for (size_t i = 0; i < students.size(); ++i) {
        const auto &s = students[i];
        cout << i << ": " << s.surname << " " << s.group << " " << s.m1 << " " << s.m2 << " " << s.m3 << " " << fixed << setprecision(2) << s.avg << endl;
    }
    cout << "==================" << endl;
}

void AppendToFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    ofstream out(fileName, ios::app);
    if (!out) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    cout << "Введите фамилию и группу: " << endl;
    string surname, group;
    cin >> surname >> group;
    int m1, m2, m3;
    cout << "Введите оценки по физике, математике и информатике: " << endl;
    cin >> m1 >> m2 >> m3;
    double avg = (m1 + m2 + m3) / 3.0;
    out << surname << " " << group << " " << m1 << " " << m2 << " " << m3 << " " << fixed << setprecision(2) << avg << endl;
    cout << "Данные добавлены успешно." << endl;
}

void EditFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    vector<Student> students;
    if (!readStudentsFromFile(fileName, students)) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    cout << "\nСодержимое файла:" << endl;
    cout << "==================" << endl;
    for (size_t i = 0; i < students.size(); ++i) {
        const auto &s = students[i];
        cout << i << ": " << s.surname << " " << s.group << " " << s.m1 << " " << s.m2 << " " << s.m3 << " " << fixed << setprecision(2) << s.avg << endl;
    }
    cout << "==================" << endl;
    bool editing = true;
    while (editing) {
        cout << "\nВыберите действие:" << endl;
        cout << "1. Редактировать строку" << endl;
        cout << "2. Выход" << endl;
        cout << "Ввод: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            cout << "Введите номер строки для редактирования: ";
            int idx; cin >> idx;
            if (idx >= 0 && idx < (int)students.size()) {
                cout << "Текущий текст: " << students[idx].surname << " " << students[idx].group << " " << students[idx].m1 << " " << students[idx].m2 << " " << students[idx].m3 << " " << fixed << setprecision(2) << students[idx].avg << endl;
                cout << "Введите фамилию и группу: " << endl;
                string surname, group; cin >> surname >> group;
                cout << "Введите оценки по физике, математике и информатике: " << endl;
                int a,b,c; cin >> a >> b >> c;
                students[idx].surname = surname;
                students[idx].group = group;
                students[idx].m1 = a;
                students[idx].m2 = b;
                students[idx].m3 = c;
                students[idx].avg = (a + b + c) / 3.0;
                cout << "Строка обновлена." << endl;
            } else {
                cout << "Неверный номер строки." << endl;
            }
        } else if (choice == 2) {
            if (!writeStudentsToFile(fileName, students)) cout << "Ошибка при сохранении файла." << endl;
            else cout << "Файл сохранён успешно." << endl;
            editing = false;
        } else {
            cout << "Неверный выбор." << endl;
        }
    }
}

void DeleteFromFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    vector<Student> students;
    if (!readStudentsFromFile(fileName, students)) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    cout << "\nСодержимое файла:" << endl;
    cout << "==================" << endl;
    for (size_t i = 0; i < students.size(); ++i) {
        const auto &s = students[i];
        cout << i << ": " << s.surname << " " << s.group << " " << s.m1 << " " << s.m2 << " " << s.m3 << " " << fixed << setprecision(2) << s.avg << endl;
    }
    cout << "==================" << endl;
    cout << "Введите номер строки для удаления: ";
    int idx; cin >> idx;
    if (idx >= 0 && idx < (int)students.size()) {
        students.erase(students.begin() + idx);
        if (!writeStudentsToFile(fileName, students)) cout << "Ошибка при сохранении файла." << endl;
        else cout << "Файл сохранён успешно." << endl;
    } else {
        cout << "Неверный номер строки." << endl;
    }
}

void SortFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    vector<Student> students;
    if (!readStudentsFromFile(fileName, students)) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    cout << "Выберите режим сортировки:\n";
    cout << "1 - Алфавитная (по фамилии)\n";
    cout << "2 - По среднему (возрастание)\n";
    cout << "3 - По среднему (убывание)\n";
    cout << "Ввод: ";
    int mode; cin >> mode;
    size_t n = students.size();
    for (size_t pass = 0; pass + 1 < n; ++pass) {
        bool swapped = false;
        for (size_t j = 0; j + 1 < n - pass; ++j) {
            bool should_swap = false;
            if (mode == 1) {
                if (students[j].surname > students[j+1].surname) should_swap = true;
            } else if (mode == 2) {
                if (students[j].avg > students[j+1].avg) should_swap = true;
            } else if (mode == 3) {
                if (students[j].avg < students[j+1].avg) should_swap = true;
            }
            if (should_swap) {
                Student tmp = students[j];
                students[j] = students[j+1];
                students[j+1] = tmp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    if (!writeStudentsToFile(fileName, students)) cout << "Ошибка при сохранении файла." << endl;
    else cout << "Файл отсортирован и сохранён успешно." << endl;
}

void SolveTask() {
    cout << "Введите имя файла для задания: ";
    string fileName;
    cin >> fileName;
    vector<Student> students;
    if (!readStudentsFromFile(fileName, students)) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    cout << "Введите номер группы для поиска отличников: ";
    string targetGroup; cin >> targetGroup;
    cout << "Введите имя выходного файла: ";
    string userName; cin >> userName;
    
    vector<Student> result;
    for (const auto &s : students) {
        if (s.group == targetGroup && s.avg >= 9.0) result.push_back(s);
    }
    if (result.empty()) {
        cout << "Нет студентов в группе " << targetGroup << " с средним баллом >= 9." << endl;
        return;
    }
    for (size_t i = 0; i + 1 < result.size(); ++i) {
        size_t maxIdx = i;
        for (size_t j = i + 1; j < result.size(); ++j) {
            if (result[j].avg > result[maxIdx].avg) maxIdx = j;
        }
        if (maxIdx != i) swap(result[i], result[maxIdx]);
    }
    
    string outputFileName = userName + "_result.txt";
    ofstream outFile(outputFileName);
    if (!outFile) {
        cout << "Ошибка при создании выходного файла." << endl;
        return;
    }
    
    outFile << "Студенты группы " << targetGroup << " отличники, отсортированные по убыванию среднего балла:\n";
    outFile << "==============================================================" << endl;
    for (const auto &s : result) {
        outFile << s.surname << " " << s.group << " " << s.m1 << " " << s.m2 << " " << s.m3 << " " << fixed << setprecision(2) << s.avg << endl;
    }
    outFile << "==============================================================" << endl;
    outFile.close();
    
    cout << "\nСтуденты группы " << targetGroup << " отличники, отсортированные по убыванию среднего балла:\n";
    cout << "==============================================================" << endl;
    for (const auto &s : result) cout << s.surname << " " << s.group << " " << s.m1 << " " << s.m2 << " " << s.m3 << " " << fixed << setprecision(2) << s.avg << endl;
    cout << "==============================================================" << endl;
    cout << "Результаты сохранены в файл: " << outputFileName << endl;
}
