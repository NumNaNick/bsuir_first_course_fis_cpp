#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
 
using namespace std;

void CreateFile();
void ViewFile();
void AppendToFile();
void EditFile();
void SolveTask();
void DeleteFromFile();
void SortFile();

int main() {
    setlocale(LC_ALL, "RU");
    bool run = true;
    while (run) {
        cout << 
        "Выберите действие:" << endl <<
        "1. Создание" << endl <<
        "2. Просмотр" << endl <<
        "3. Добавление" << endl <<
        "4. Решение индивидуального задания" << endl <<
        "5. Редактирование" << endl <<
        "6. Удаление" << endl <<
        "7. Сортировка" << endl <<
        "8. Выход из программы" << endl <<
        "Ввод данных: ";
        int action;
        cin >> action;
        switch(action) {
            case 1: CreateFile(); break;
            case 2: ViewFile(); break;
            case 3: AppendToFile(); break;
            case 4: SolveTask(); break;
            case 5: EditFile(); break;
            case 6: DeleteFromFile(); break;
            case 7: SortFile(); break;
            case 8: run = false; break;
        }
    }
}

void CreateFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;

    ofstream file(fileName);
    if (!file) {
        cout << "Ошибка при создании файла." << endl;
        return;
    }
    cout << "Файл создан успешно." << endl;
    file.close();
}

void ViewFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    
    ifstream file(fileName);
    if (!file) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }

    // Read all lines from file into a vector
    vector<string> fileContent;
    string line;
    while (getline(file, line)) {
        fileContent.push_back(line);
    }
    file.close();

    // Display the content
    cout << "\nСодержимое файла:" << endl;
    cout << "==================" << endl;
    for (size_t i = 0; i < fileContent.size(); i++) {
        cout << i << ": " << fileContent[i] << endl;
    }
    cout << "==================" << endl;
}

void AppendToFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;

    ofstream file(fileName, ios::app);
    if (!file) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }

    cout << "Введите фамилию и группу(через пробел): " << endl;
    string data;
    cin.ignore(); // Очистка буфера ввода
    getline(cin, data);
    file << data << " ";

    int array[3]; 
    cout << "Введите оценки по физике, математике и информатике: " << endl;
    for (int i = 0; i < 3; i++) {
        cin >> array[i];
    }
    for (int i = 0; i < 3; i++) {
        file << array[i] << " ";
    }
    file << fixed << setprecision(2) << (array[0] + array[1] + array[2])/3.0 << endl;
    cout << "Данные добавлены успешно." << endl;
    file.close();
}

void SolveTask() { 
    cout << "Введите имя файла для задания: ";
    string fileName;
    cin >> fileName;

    ifstream file(fileName);
    if (!file) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }

    // Read all lines
    vector<string> fileContent;
    string line;
    while (getline(file, line)) fileContent.push_back(line);
    file.close();

    // Ask user for the group number to search for
    cout << "Введите номер группы для поиска отличников: ";
    string targetGroup;
    cin >> targetGroup;

    // Helper: extract last numeric token from a line
    auto extract_last_number = [](const string &s) -> double {
        int i = (int)s.size() - 1;
        // skip trailing whitespace
        while (i >= 0 && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i--;
        if (i < 0) return 0.0;
        int end = i;
        // find start of last token
        while (i >= 0 && !(s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i--;
        int start = i + 1;
        string token = s.substr(start, end - start + 1);
        try { return stod(token); } catch (...) { return 0.0; }
    };

    // Helper: extract group number (second token) from a line
    auto extract_group = [](const string &s) -> string {
        vector<string> tokens;
        string tok;
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i];
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                if (!tok.empty()) { tokens.push_back(tok); tok.clear(); }
            } else tok.push_back(c);
        }
        if (!tok.empty()) tokens.push_back(tok);

        // return second token (group number), or empty if not enough tokens
        if (tokens.size() >= 2) return tokens[1];
        return "";
    };

    // Helper: extract first token (surname) from a line
    auto extract_surname = [](const string &s) -> string {
        vector<string> tokens;
        string tok;
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i];
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                if (!tok.empty()) { tokens.push_back(tok); tok.clear(); }
            } else tok.push_back(c);
        }
        if (!tok.empty()) tokens.push_back(tok);

        // return first token (surname), or empty if no tokens
        if (tokens.size() >= 1) return tokens[0];
        return "";
    };

    // Filter lines: group number matches targetGroup and mark4 > 9
    vector<string> result;
    for (const string &ln : fileContent) {
        string group = extract_group(ln);
        double lastNum = extract_last_number(ln);
        if (group == targetGroup && lastNum >= 9.0) {
            result.push_back(ln);
        }
    }

    if (result.empty()) {
        cout << "Нет студентов в группе " << targetGroup << " с средним баллом >= 9." << endl;
        return;
    }

    // Sort result by decreasing lastNum (simple selection sort, same as SortFile mode 3)
    for (size_t i = 0; i + 1 < result.size(); ++i) {
        size_t maxIdx = i;
        for (size_t j = i + 1; j < result.size(); ++j) {
            double last_j = extract_last_number(result[j]);
            double last_maxIdx = extract_last_number(result[maxIdx]);
            if (last_j > last_maxIdx) maxIdx = j;
        }
        if (maxIdx != i) {
            string tmp = result[i];
            result[i] = result[maxIdx];
            result[maxIdx] = tmp;
        }
    }

    // Display results
    cout << "\nСтуденты группы " << targetGroup << " отличники, отсортированные по убыванию среднего балла:\n";
    cout << "==============================================================" << endl;
    for (const auto &e : result) {
        double mark4 = extract_last_number(e);
        cout << e << endl;
    }
    cout << "==============================================================" << endl;
}

void EditFile() { 
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;
    
    ifstream file(fileName);
    if (!file) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }

    // Read all lines from file into a vector
    vector<string> fileContent;
    string line;
    while (getline(file, line)) {
        fileContent.push_back(line);
    }
    file.close();

    // Display the content
    cout << "\nСодержимое файла:" << endl;
    cout << "==================" << endl;
    for (size_t i = 0; i < fileContent.size(); i++) {
        cout << i << ": " << fileContent[i] << endl;
    }
    cout << "==================" << endl;

    // Allow user to edit lines
    bool editing = true;
    while (editing) {
        cout << "\nВыберите действие:" << endl;
        cout << "1. Редактировать строку" << endl;
        cout << "2. Выход" << endl;
        cout << "Ввод: ";
        int choice;
        cin >> choice;
        cin.ignore(); // Clear input buffer

        if (choice == 1) {
            // Edit existing line
            cout << "Введите номер строки для редактирования: ";
            int lineNum;
            cin >> lineNum;
            cin.ignore();

            if (lineNum >= 0 && lineNum < fileContent.size()) {
                cout << "Текущий текст: " << fileContent[lineNum] << endl;
                cout << "Введите фамилию и группу(через пробел): " << endl;
                string data;
                getline(cin, data);
                fileContent[lineNum] = data;
                
                int array[3];
                cout << "Введите оценки по физике, математике и информатике: " << endl;
                for (int i = 0; i < 3; i++) {
                    cin >> array[i];
                }
                cin.ignore(); // Clear input buffer after reading integers
                fileContent[lineNum] = fileContent[lineNum] + " ";
                for (int i = 0; i < 3; i++) {
                    fileContent[lineNum] += to_string(array[i]) + " ";
                }
                // Format average with setprecision(2)
                stringstream ss;
                ss << fixed << setprecision(2) << (array[0] + array[1] + array[2])/3.0;
                fileContent[lineNum] += ss.str();
                cout << "Строка обновлена." << endl;
                
                // Display current content
                cout << "\nТекущее содержимое:" << endl;
                cout << "==================" << endl;
                for (size_t i = 0; i < fileContent.size(); i++) {
                    cout << i << ": " << fileContent[i] << endl;
                }
                cout << "==================" << endl;
            } else {
                cout << "Неверный номер строки." << endl;
            }
        } 
        else if (choice == 2) {
            // Save and exit
            ofstream outputFile(fileName);
            if (!outputFile) {
                cout << "Ошибка при сохранении файла." << endl;
                return;
            }

            for (const string& s : fileContent) {
                outputFile << s << endl;
            }
            outputFile.close();
            cout << "Файл сохранён успешно." << endl;
            editing = false;
        } 
        else {
            cout << "Неверный выбор." << endl;
        }
    }
}

void DeleteFromFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;

    ifstream file(fileName);
    if (!file) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }

    // Read all lines from file into a vector
    vector<string> fileContent;
    string line;
    while (getline(file, line)) {
        fileContent.push_back(line);
    }
    file.close();

    // Display the content
    cout << "\nСодержимое файла:" << endl;
    cout << "==================" << endl;
    for (size_t i = 0; i < fileContent.size(); i++) {
        cout << i << ": " << fileContent[i] << endl;
    }
    cout << "==================" << endl;

    // Allow user to delete lines
    cout << "Введите номер строки для удаления: ";
    int lineNum;
    cin >> lineNum;

    if (lineNum >= 0 && lineNum < fileContent.size()) {
        fileContent.erase(fileContent.begin() + lineNum);
        cout << "Строка удалена." << endl;

        // Save changes back to the file
        ofstream outputFile(fileName);
        if (!outputFile) {
            cout << "Ошибка при сохранении файла." << endl;
            return;
        }

        for (const string& s : fileContent) {
            outputFile << s << endl;
        }
        outputFile.close();
        cout << "Файл сохранён успешно." << endl;
    } else {
        cout << "Неверный номер строки." << endl;
    }
}

void SortFile() {
    cout << "Введите имя файла: ";
    string fileName;
    cin >> fileName;

    ifstream file(fileName);
    if (!file) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }

    // Read all lines from file into a vector
    vector<string> fileContent;
    string line;
    while (getline(file, line)) {
        fileContent.push_back(line);
    }
    file.close();

    // Choose sort mode
    cout << "Выберите режим сортировки:\n";
    cout << "1 - Алфавитная (по строке)\n";
    cout << "2 - По последнему числу (возрастание)\n";
    cout << "3 - По последнему числу (убывание)\n";
    cout << "Ввод: ";
    int mode = 1;
    cin >> mode;

    // Helper: extract last token and try to convert to double
    auto extract_last_number = [](const string &s) -> double {
        int i = (int)s.size() - 1;
        // skip trailing whitespace characters (space, tab, CR, LF)
        while (i >= 0 && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i--;
        if (i < 0) return 0.0;
        int end = i;
        // find start of last token
        while (i >= 0 && !(s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i--;
        int start = i + 1;
        string token = s.substr(start, end - start + 1);
        try { return stod(token); } catch (...) { return 0.0; }
    };

    // Simple in-place stable-like bubble sort so we don't need <algorithm>
    size_t n = fileContent.size();
    for (size_t pass = 0; pass + 1 < n; ++pass) {
        bool swapped = false;
        for (size_t j = 0; j + 1 < n - pass; ++j) {
            bool should_swap = false;
            if (mode == 1) {
                // alphabetical (lexicographical) ascending
                if (fileContent[j] > fileContent[j+1]) should_swap = true;
            } else if (mode == 2) {
                double a = extract_last_number(fileContent[j]);
                double b = extract_last_number(fileContent[j+1]);
                if (a > b) should_swap = true;
            } else if (mode == 3) {
                double a = extract_last_number(fileContent[j]);
                double b = extract_last_number(fileContent[j+1]);
                if (a < b) should_swap = true;
            }
            if (should_swap) {
                string tmp = fileContent[j];
                fileContent[j] = fileContent[j+1];
                fileContent[j+1] = tmp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }

    // Save sorted content back to the file
    ofstream outputFile(fileName);
    if (!outputFile) {
        cout << "Ошибка при сохранении файла." << endl;
        return;
    }

    for (const string& s : fileContent) {
        outputFile << s << endl;
    }
    outputFile.close();
    cout << "Файл отсортирован и сохранён успешно." << endl;
}