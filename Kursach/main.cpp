#include <iostream>

using namespace std;

#define FILENAME "tickets.dat"
#define REPORT_FILE "report.txt"

struct Ticket {
    char zone[50];
    int free_seats;
    float price;
    char chair_type[30];
    int is_discount_allowed;
};

int getValidInt() {
    int value;
    while (!(cin >> value)) {
        cout << "Ошибка! Введите целое число: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    return value;
}

float getValidFloat() {
    float value;
    while (!(cin >> value)) {
        cout << "Ошибка! Введите дробное число (через точку): ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    return value;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void clearScreen() { 
    system("clear");
    //system("cls"); 
}

void waitForEnter() {
    cout << "\nНажмите Enter...";
    cin.get();
}

void printTicket(Ticket t) {
    cout << "Зона: " << t.zone << " | Мест: " << t.free_seats << 
    " | Цена: " << t.price << " | Тип: " << t.chair_type << 
    " | Льгота: " << (t.is_discount_allowed ? "Да" : "Нет") << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;
}

Ticket getAt(FILE* f, int i) {
    Ticket t;
    fseek(f, i * sizeof(Ticket), SEEK_SET);
    fread(&t, sizeof(Ticket), 1, f);
    return t;
}

void setAt(FILE* f, int i, Ticket t) {
    fseek(f, i * sizeof(Ticket), SEEK_SET);
    fwrite(&t, sizeof(Ticket), 1, f);
}

int getCount(FILE* f) {
    fseek(f, 0, SEEK_END);
    return ftell(f) / sizeof(Ticket);
}

void linearSearchBySeats(int key) {
    FILE* f = fopen(FILENAME, "rb");
    if (!f) return;
    Ticket t; bool found = false;
    while (fread(&t, sizeof(Ticket), 1, f) == 1) {
        if (t.free_seats == key) { printTicket(t); found = true; }
    }
    if (!found) cout << "Места не найдены." << endl;
    fclose(f);
}

void interpolationSearchByPrice(float key) {
    FILE* f = fopen(FILENAME, "rb");
    if (!f) return;
    int low = 0, high = getCount(f) - 1;
    bool found = false;
    while (low <= high && key >= getAt(f, low).price && key <= getAt(f, high).price) {
        if (getAt(f, high).price == getAt(f, low).price) {
            if (getAt(f, low).price == key) { printTicket(getAt(f, low)); found = true; }
            break;
        }
        int pos = low + (((double)(high - low) / (getAt(f, high).price - getAt(f, low).price)) * (key - getAt(f, low).price));
        Ticket tPos = getAt(f, pos);
        if (tPos.price == key) { printTicket(tPos); found = true; break; }
        if (tPos.price < key) low = pos + 1; else high = pos - 1;
    }
    if (!found) cout << "Билеты не найдены." << endl;
    fclose(f);
}

void quickSortByZone(FILE* f, int left, int right) {
    int i = left, j = right;
    Ticket pivot = getAt(f, (left + right) / 2);
    while (i <= j) {
        while (strcmp(getAt(f, i).zone, pivot.zone) < 0) i++;
        while (strcmp(getAt(f, j).zone, pivot.zone) > 0) j--;
        if (i <= j) {
            Ticket ti = getAt(f, i), tj = getAt(f, j);
            setAt(f, i, tj); setAt(f, j, ti);
            i++; j--;
        }
    }
    if (left < j) quickSortByZone(f, left, j);
    if (i < right) quickSortByZone(f, i, right);
}

void insertionSortBySeats() {
    FILE* f = fopen(FILENAME, "rb+");
    if (!f) return;
    int n = getCount(f);
    for (int i = 1; i < n; i++) {
        Ticket key = getAt(f, i);
        int j = i - 1;
        while (j >= 0 && getAt(f, j).free_seats > key.free_seats) {
            setAt(f, j + 1, getAt(f, j));
            j--;
        }
        setAt(f, j + 1, key);
    }
    fclose(f);
}

void bubbleSortByPrice() {
    FILE* f = fopen(FILENAME, "rb+");
    if (!f) return;
    int n = getCount(f);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Ticket t1 = getAt(f, j), t2 = getAt(f, j + 1);
            if (t1.price > t2.price) { setAt(f, j, t2); setAt(f, j + 1, t1); }
        }
    }
    fclose(f);
}

void statisticsByChairType() {
    FILE* f = fopen(FILENAME, "rb");
    FILE* report = fopen(REPORT_FILE, "a");

    if (!f) {
        cout << "Ошибка: База данных не найдена!" << endl;
        if (report) fclose(report);
        return;
    }

    int n = getCount(f);
    if (n == 0) {
        cout << "Файл пуст." << endl;
        fclose(f);
        if (report) fclose(report);
        return;
    }

    if (report) {
        fprintf(report, "\n=== СТАТИСТИКА ПО ТИПАМ КРЕСЕЛ ===\n");
        fprintf(report, "------------------------------------------\n");
    }

    cout << "\nСтатистика по типам кресел:" << endl;
    cout << "------------------------------------------" << endl;

    for (int i = 0; i < n; i++) {
        Ticket current = getAt(f, i);
        
        bool alreadyProcessed = false;
        for (int j = 0; j < i; j++) {
            Ticket previous = getAt(f, j);
            if (strcmp(current.chair_type, previous.chair_type) == 0) {
                alreadyProcessed = true;
                break;
            }
        }

        if (!alreadyProcessed) {
            float sumPrice = 0;
            int countTickets = 0;

            for (int k = 0; k < n; k++) {
                Ticket check = getAt(f, k);
                if (strcmp(check.chair_type, current.chair_type) == 0) {
                    sumPrice += check.price;
                    countTickets++;
                }
            }

            float average = sumPrice / countTickets;

            cout << "Тип: " << current.chair_type << " | Средняя цена: " << average << endl;

            if (report) {
                fprintf(report, "Тип: %-15s | Средняя цена: %-10.2f\n", current.chair_type, average);
            }
        }
    }

    fclose(f);
    if (report) {
        fprintf(report, "------------------------------------------\n");
        fclose(report);
        cout << "\nСтатистика успешно сохранена в " << REPORT_FILE << endl;
    }
}

void deleteTicket() {
    FILE* f = fopen(FILENAME, "rb");
    if (!f) return;
    int i = 1, ch; Ticket t;
    while (fread(&t, sizeof(Ticket), 1, f) == 1) cout << i++ << ". " << t.zone << endl;
    cout << "Удалить номер: "; cin >> ch;
    if (ch > 0) {
        rewind(f); FILE* tmp = fopen("temp.dat", "wb");
        int cur = 1;
        while (fread(&t, sizeof(Ticket), 1, f) == 1) {
            if (cur != ch) fwrite(&t, sizeof(Ticket), 1, tmp);
            cur++;
        }
        fclose(f); fclose(tmp); remove(FILENAME); rename("temp.dat", FILENAME);
    } else fclose(f);
}

void findSuitableSeat(int neededSeats, int discountFlag) {
    bubbleSortByPrice(); 

    FILE* f = fopen(FILENAME, "rb");
    FILE* report = fopen(REPORT_FILE, "w");

    if (!f) {
        cout << "Ошибка: база данных не найдена!" << endl;
        if (report) fclose(report);
        return;
    }

    if (report) {
        fprintf(report, "=== ОТЧЕТ: ПОДХОДЯЩИЕ МЕСТА ===\n");
        fprintf(report, "Запрос: мест >= %d, льгота = %s\n", 
                neededSeats, (discountFlag ? "Да" : "Нет"));
        fprintf(report, "------------------------------------------\n");
    }

    Ticket t;
    bool found = false;
    cout << "\nРезультаты (сортировка по возрастанию цены):" << endl;

    while (fread(&t, sizeof(Ticket), 1, f) == 1) {
        if (t.free_seats >= neededSeats && t.is_discount_allowed == discountFlag) {
            printTicket(t);

            if (report) {
                fprintf(report, "Зона: %-15s | Мест: %-3d | Цена: %-8.2f | Тип: %s\n",
                        t.zone, t.free_seats, t.price, t.chair_type);
            }
            found = true;
        }
    }

    if (!found) {
        cout << "Подходящих вариантов не найдено." << endl;
        if (report) fprintf(report, "Варианты не найдены.\n");
    } else {
        cout << "\nОтчет сохранен в " << REPORT_FILE << endl;
    }

    fclose(f);
    if (report) {
        fprintf(report, "------------------------------------------\n");
        fclose(report);
    }
}

void addTicket() {
    clearScreen();
    Ticket t;
    cout << "=== ДОБАВЛЕНИЕ БИЛЕТА ===\n";
    cout << "Зона: "; cin.getline(t.zone, 50);
    
    cout << "Свободных мест: ";
    t.free_seats = getValidInt();
    
    cout << "Стоимость: ";
    t.price = getValidFloat();
    
    cout << "Тип кресел: "; 
    cin.getline(t.chair_type, 30);
    
    cout << "Льгота (1-да, 0-нет): ";
    int value;
    while (!(cin >> value) || (value != 0 && value != 1)) {
        cout << "Ошибка! Введите целое число: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    t.is_discount_allowed = value;

    FILE* f = fopen(FILENAME, "ab");
    if (f) { fwrite(&t, sizeof(Ticket), 1, f); fclose(f); }
}

int main() {
    setlocale(LC_ALL, "RU");
    int choice;
    do {
        clearScreen();
        cout << "========================================\n";
        cout << "    УЧЕТ БИЛЕТОВ НА КОНЦЕРТ\n";
        cout << "========================================\n";
        cout << "1. Добавить билет\n";
        cout << "2. Показать все билеты\n";
        cout << "3. Удалить билет\n";
        cout << "4. Линейный поиск по кол-ву мест\n";
        cout << "5. Интерполяционный поиск по цене\n";
        cout << "6. Быстрая сортировка по зоне\n";
        cout << "7. Сортировка вставками по кол-ву мест\n";
        cout << "8. Сортировка пузырьком по цене\n";
        cout << "9. Поиск подходящего места\n";
        cout << "10. Статистика по типу кресел\n";
        cout << "11. Выход\n";
        cout << "========================================\n";
        cout << "Выберите действие: ";
        if (!(cin >> choice)) { 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            continue; 
        }
        cin.ignore();
        
        switch (choice) {
            case 1: { 
                clearScreen(); 
                addTicket(); 
                break; 
            }
            case 2: { clearScreen(); 
                FILE* f = fopen(FILENAME, "rb"); 
                if(f) { 
                    Ticket t; 
                    while(fread(&t, sizeof(Ticket), 1, f)) printTicket(t); 
                    fclose(f); 
                } waitForEnter(); break; }
            case 3: clearScreen(); deleteTicket(); break;
            case 4: { 
                cout << "Места: "; 
                linearSearchBySeats(getValidInt()); 
                waitForEnter(); 
                break; 
            }
            case 5: { 
                cout << "Цена: "; 
                bubbleSortByPrice();
                interpolationSearchByPrice(getValidFloat()); 
                waitForEnter(); 
                break; 
            }
            case 6: { 
                FILE* f = fopen(FILENAME, "rb+"); 
                if(f) { 
                    quickSortByZone(f, 0, getCount(f)-1); 
                    fclose(f); 
                } 
                break; 
            }
            case 7: insertionSortBySeats(); break;
            case 8: bubbleSortByPrice(); break;
            case 9: { 
                clearScreen();
                cout << "=== ПОИСК ПОДХОДЯЩИХ МЕСТ ===" << endl;
                cout << "Введите минимальное количество мест: "; 
                int m = getValidInt();
                
                cout << "Нужна ли льгота? (1 - да, 0 - нет): ";
                int d = getValidInt();
                
                while (d < 0 || d > 1) {
                    cout << "Ошибка! Введите 1 (да) или 0 (нет): ";
                    d = getValidInt();
                }

                findSuitableSeat(m, d); 
                waitForEnter(); 
                break; 
            }
            case 10: clearScreen(); statisticsByChairType(); waitForEnter(); break;
        }
    } while (choice != 11);
    return 0;
}