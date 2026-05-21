/*
Лабораторная работа №4. Динамическая структура ОЧЕРЕДЬ.
Вариант 14: из созданного списка удалить каждый второй элемент.
*/

#include <iostream>
#include <locale>

using namespace std;

struct Spis2 {
    int info;
    Spis2* prev;
    Spis2* next;
};

int inputInt(const char* text) {
    int x;
    cout << text;
    while (!(cin >> x)) {
        cout << "Ошибка ввода. Введите целое число: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return x;
}

int randomNumber(unsigned int& seed) {
    seed = seed * 1103515245u + 12345u;
    return int((seed / 65536u) % 101) - 50; // число от -50 до 50
}

void AddEnd(Spis2** begin, Spis2** end, int in) {
    Spis2* t = new Spis2;
    t->info = in;
    t->next = NULL;
    t->prev = *end;

    if (*begin == NULL) {
        *begin = t;
        *end = t;
    } else {
        (*end)->next = t;
        *end = t;
    }
}

void AddBegin(Spis2** begin, Spis2** end, int in) {
    Spis2* t = new Spis2;
    t->info = in;
    t->prev = NULL;
    t->next = *begin;

    if (*begin == NULL) {
        *begin = t;
        *end = t;
    } else {
        (*begin)->prev = t;
        *begin = t;
    }
}

void ViewBegin(Spis2* begin) {
    if (begin == NULL) {
        cout << "Очередь пуста.\n";
        return;
    }

    Spis2* t = begin;
    cout << "Просмотр с начала: ";
    while (t != NULL) {
        cout << t->info << " ";
        t = t->next;
    }
    cout << endl;
}

void ViewEnd(Spis2* end) {
    if (end == NULL) {
        cout << "Очередь пуста.\n";
        return;
    }

    Spis2* t = end;
    cout << "Просмотр с конца: ";
    while (t != NULL) {
        cout << t->info << " ";
        t = t->prev;
    }
    cout << endl;
}

void DeleteNode(Spis2** begin, Spis2** end, Spis2* key) {
    if (key == NULL) return;

    if (key->prev != NULL) key->prev->next = key->next;
    else *begin = key->next;

    if (key->next != NULL) key->next->prev = key->prev;
    else *end = key->prev;

    delete key;
}

void DeleteEverySecond(Spis2** begin, Spis2** end) {
    if (*begin == NULL || (*begin)->next == NULL) {
        cout << "Недостаточно элементов для удаления.\n";
        return;
    }

    Spis2* current = *begin;
    while (current != NULL && current->next != NULL) {
        Spis2* del = current->next;   // удаляем второй после текущего
        current = del->next;          // переходим к следующему оставшемуся
        DeleteNode(begin, end, del);
    }
}

void DelAll(Spis2** begin, Spis2** end) {
    Spis2* t;
    while (*begin != NULL) {
        t = *begin;
        *begin = (*begin)->next;
        delete t;
    }
    *end = NULL;
}

int main() {
    setlocale(LC_ALL, "Russian");

    Spis2* begin = NULL;
    Spis2* end = NULL;
    unsigned int seed = 1;
    int choice, n, value;

    while (true) {
        cout << "\n========== МЕНЮ ==========" << endl;
        cout << "1. Создать очередь случайных чисел" << endl;
        cout << "2. Добавить элемент в начало" << endl;
        cout << "3. Добавить элемент в конец" << endl;
        cout << "4. Просмотреть с начала" << endl;
        cout << "5. Просмотреть с конца" << endl;
        cout << "6. Удалить каждый второй элемент" << endl;
        cout << "7. Очистить очередь" << endl;
        cout << "0. Выход" << endl;

        choice = inputInt("Ваш выбор: ");

        switch (choice) {
            case 1:
                DelAll(&begin, &end);
                n = inputInt("Количество элементов: ");
                while (n <= 0) n = inputInt("Количество должно быть больше 0: ");
                seed = (unsigned int)inputInt("Введите любое число для генерации: ");
                for (int i = 0; i < n; i++) {
                    AddEnd(&begin, &end, randomNumber(seed));
                }
                cout << "Очередь создана.\n";
                ViewBegin(begin);
                break;

            case 2:
                value = inputInt("Введите значение: ");
                AddBegin(&begin, &end, value);
                cout << "Элемент добавлен в начало.\n";
                break;

            case 3:
                value = inputInt("Введите значение: ");
                AddEnd(&begin, &end, value);
                cout << "Элемент добавлен в конец.\n";
                break;

            case 4:
                ViewBegin(begin);
                break;

            case 5:
                ViewEnd(end);
                break;

            case 6:
                cout << "До удаления:\n";
                ViewBegin(begin);
                DeleteEverySecond(&begin, &end);
                cout << "После удаления:\n";
                ViewBegin(begin);
                break;

            case 7:
                DelAll(&begin, &end);
                cout << "Память очищена.\n";
                break;

            case 0:
                DelAll(&begin, &end);
                return 0;

            default:
                cout << "Такого пункта меню нет.\n";
        }
    }
}
