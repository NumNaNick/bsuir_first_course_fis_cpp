#include <iostream>
#include <stdlib.h>

using namespace std;

#define FILENAME "tickets.dat"
#define REPORT_FILE "report.txt"
#define MAX_LEN 100
#define MAX_TICKETS 1000

struct Ticket {
    char zone[50];
    int free_seats;
    float price;
    char chair_type[30];
    int is_discount_allowed;
};

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void strcpy(char* dest, const char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void clearScreen() {
    system("cls");
}

void waitForEnter() {
    cout << "\nНажмите Enter чтобы продолжить...";
    cin.ignore(1000, '\n');
    cin.ignore();
    cin.get();
}

int getFileSize() {
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) return 0;
    fseek(f, 0, SEEK_END);
    int size = ftell(f) / sizeof(struct Ticket);
    fclose(f);
    return size;
}

void printTicket(struct Ticket t) {
    cout << "----------------------------------------" << endl;
    cout << "Зона: " << t.zone << endl;
    cout << "Свободных мест: " << t.free_seats << endl;
    cout << "Стоимость: " << t.price << endl;
    cout << "Тип кресел: " << t.chair_type << endl;
    cout << "Льготный билет: " << (t.is_discount_allowed ? "Да" : "Нет") << endl;
}

void addTicket() {
    clearScreen();
    struct Ticket newTicket;
    FILE *f;
    
    cout << "\n=== ДОБАВЛЕНИЕ НОВОГО БИЛЕТА ===\n\n";
    
    cout << "Введите зону расположения: ";
    cin.getline(newTicket.zone, 50);
    
    cout << "Введите количество свободных мест: ";
    cin >> newTicket.free_seats;
    cin.ignore();
    
    cout << "Введите стоимость билета: ";
    cin >> newTicket.price;
    cin.ignore();
    
    cout << "Введите тип кресел: ";
    cin.getline(newTicket.chair_type, 30);
    
    cout << "Разрешена ли льгота? (1-да, 0-нет): ";
    cin >> newTicket.is_discount_allowed;
    cin.ignore();
    
    f = fopen(FILENAME, "ab");
    if (f == NULL) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }
    
    fwrite(&newTicket, sizeof(struct Ticket), 1, f);
    fclose(f);
    
    cout << "\nБилет успешно добавлен!" << endl;
    waitForEnter();
}

void viewTickets() {
    clearScreen();
    struct Ticket t;
    FILE *f;
    int count = 0;
    
    cout << "\n=== ВСЕ БИЛЕТЫ ===\n\n";
    
    f = fopen(FILENAME, "rb");
    if (f == NULL) {
        cout << "Файл с билетами не найден." << endl;
        waitForEnter();
        return;
    }
    
    while (fread(&t, sizeof(struct Ticket), 1, f) == 1) {
        count++;
        printTicket(t);
    }
    
    fclose(f);
    
    if (count == 0) {
        cout << "Билетов пока нет." << endl;
    } else {
        cout << "----------------------------------------" << endl;
        cout << "Всего билетов: " << count << endl;
    }
    
    waitForEnter();
}

void deleteTicket() {
    clearScreen();
    struct Ticket t;
    FILE *f, *temp;
    int choice, index;
    
    cout << "\n=== УДАЛЕНИЕ БИЛЕТА ===\n\n";
    
    f = fopen(FILENAME, "rb");
    if (f == NULL) {
        cout << "Файл с билетами не найден." << endl;
        waitForEnter();
        return;
    }
    
    // Загружаем все билеты в массив
    struct Ticket *allTickets = (struct Ticket*)malloc(MAX_TICKETS * sizeof(struct Ticket));
    int count = 0;
    
    cout << "Доступные зоны для удаления:" << endl;
    cout << "----------------------------------------" << endl;
    
    while (fread(&t, sizeof(struct Ticket), 1, f) == 1) {
        allTickets[count] = t;
        cout << count + 1 << ". Зона: " << t.zone << " | Свободных мест: " << t.free_seats << " | Цена: " << t.price << " | Тип: " << t.chair_type << endl;
        count++;
    }
    fclose(f);
    
    if (count == 0) {
        cout << "Нет билетов для удаления." << endl;
        free(allTickets);
        waitForEnter();
        return;
    }
    
    cout << "----------------------------------------" << endl;
    cout << "Выберите номер зоны для удаления (0 - отмена): ";
    cin >> choice;
    cin.ignore();
    
    if (choice == 0 || choice > count) {
        cout << "Удаление отменено." << endl;
        free(allTickets);
        waitForEnter();
        return;
    }
    
    index = choice - 1;
    
    cout << "\nВы выбрали:" << endl;
    cout << "Зона: " << allTickets[index].zone << endl;
    cout << "Свободных мест: " << allTickets[index].free_seats << endl;
    cout << "Цена: " << allTickets[index].price << endl;
    cout << "Тип кресел: " << allTickets[index].chair_type << endl;
    cout << "Льготный: " << (allTickets[index].is_discount_allowed ? "Да" : "Нет") << endl;
    
    cout << "\nТочно удалить? (1-да, 0-нет): ";
    int confirm;
    cin >> confirm;
    cin.ignore();
    
    if (confirm != 1) {
        cout << "Удаление отменено." << endl;
        free(allTickets);
        waitForEnter();
        return;
    }
    
    // Создаем новый файл без выбранного билета
    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        cout << "Ошибка создания временного файла." << endl;
        free(allTickets);
        waitForEnter();
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (i != index) {
            fwrite(&allTickets[i], sizeof(struct Ticket), 1, temp);
        }
    }
    
    fclose(temp);
    
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    
    cout << "\nБилет успешно удален!" << endl;
    
    free(allTickets);
    waitForEnter();
}

int linearSearchByFreeSeats(int seats, struct Ticket *results) {
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) return 0;
    
    struct Ticket t;
    int count = 0;
    
    while (fread(&t, sizeof(struct Ticket), 1, f) == 1) {
        if (t.free_seats == seats) {
            results[count] = t;
            count++;
        }
    }
    
    fclose(f);
    return count;
}

int interpolationSearchByPrice(float price, struct Ticket *arr, int n) {
    int low = 0, high = n - 1;
    
    while (low <= high && price >= arr[low].price && price <= arr[high].price) {
        if (low == high) {
            if (arr[low].price == price) return low;
            return -1;
        }
        
        int pos = low + (((double)(high - low) / (arr[high].price - arr[low].price)) * (price - arr[low].price));
        
        if (arr[pos].price == price) {
            return pos;
        }
        
        if (arr[pos].price < price) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    
    return -1;
}

void quickSortByZone(struct Ticket *arr, int low, int high) {
    if (low < high) {
        int i = low, j = high;
        struct Ticket pivot = arr[(low + high) / 2];
        
        while (i <= j) {
            while (strcmp(arr[i].zone, pivot.zone) < 0) i++;
            while (strcmp(arr[j].zone, pivot.zone) > 0) j--;
            
            if (i <= j) {
                struct Ticket temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
                j--;
            }
        }
        
        if (low < j) quickSortByZone(arr, low, j);
        if (i < high) quickSortByZone(arr, i, high);
    }
}

void insertionSortByFreeSeats(struct Ticket *arr, int n) {
    for (int i = 1; i < n; i++) {
        struct Ticket key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j].free_seats > key.free_seats) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void bubbleSortByPrice(struct Ticket *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].price > arr[j + 1].price) {
                struct Ticket temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void findSuitableSeats() {
    clearScreen();
    cout << "\n=== ПОИСК ПОДХОДЯЩЕГО МЕСТА ===\n\n";
    
    int neededSeats, needDiscount;
    cout << "Введите необходимое количество мест: ";
    cin >> neededSeats;
    cout << "Нужен ли льготный билет? (1-да, 0-нет): ";
    cin >> needDiscount;
    cin.ignore();
    
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) {
        cout << "Файл с билетами не найден." << endl;
        waitForEnter();
        return;
    }
    
    struct Ticket *allTickets = (struct Ticket*)malloc(MAX_TICKETS * sizeof(struct Ticket));
    int count = 0;
    
    while (fread(&allTickets[count], sizeof(struct Ticket), 1, f) == 1) {
        count++;
    }
    fclose(f);
    
    struct Ticket suitable[MAX_TICKETS];
    int suitableCount = 0;
    
    for (int i = 0; i < count; i++) {
        if (allTickets[i].free_seats >= neededSeats) {
            if (needDiscount == 0 || (needDiscount == 1 && allTickets[i].is_discount_allowed == 1)) {
                suitable[suitableCount] = allTickets[i];
                suitableCount++;
            }
        }
    }
    
    if (suitableCount == 0) {
        cout << "\nНет подходящих мест." << endl;
        free(allTickets);
        waitForEnter();
        return;
    }
    
    for (int i = 0; i < suitableCount - 1; i++) {
        for (int j = 0; j < suitableCount - i - 1; j++) {
            if (suitable[j].price > suitable[j + 1].price) {
                struct Ticket temp = suitable[j];
                suitable[j] = suitable[j + 1];
                suitable[j + 1] = temp;
            }
        }
    }
    
    cout << "\nПодходящие места (в порядке возрастания цены):" << endl;
    for (int i = 0; i < suitableCount; i++) {
        printTicket(suitable[i]);
    }
    
    FILE *report = fopen(REPORT_FILE, "w");
    if (report != NULL) {
        fprintf(report, "=== ОТЧЕТ: ПОИСК ПОДХОДЯЩЕГО МЕСТА ===\n");
        fprintf(report, "Необходимо мест: %d, Льготный: %s\n\n", neededSeats, needDiscount ? "Да" : "Нет");
        for (int i = 0; i < suitableCount; i++) {
            fprintf(report, "Зона: %s, Мест: %d, Цена: %.2f, Тип: %s, Льгота: %s\n",
                    suitable[i].zone, suitable[i].free_seats, suitable[i].price,
                    suitable[i].chair_type, suitable[i].is_discount_allowed ? "Да" : "Нет");
        }
        fclose(report);
        cout << "\nОтчет сохранен в файл " << REPORT_FILE << endl;
    }
    
    free(allTickets);
    waitForEnter();
}

void statisticsByChairType() {
    clearScreen();
    cout << "\n=== СТАТИСТИКА ПО ТИПУ КРЕСЕЛ ===\n\n";
    
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) {
        cout << "Файл с билетами не найден." << endl;
        waitForEnter();
        return;
    }
    
    struct Ticket *allTickets = (struct Ticket*)malloc(MAX_TICKETS * sizeof(struct Ticket));
    int count = 0;
    
    while (fread(&allTickets[count], sizeof(struct Ticket), 1, f) == 1) {
        count++;
    }
    fclose(f);
    
    char uniqueTypes[MAX_TICKETS][30];
    int typeCount = 0;
    
    for (int i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < typeCount; j++) {
            if (strcmp(allTickets[i].chair_type, uniqueTypes[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(uniqueTypes[typeCount], allTickets[i].chair_type);
            typeCount++;
        }
    }
    
    FILE *report = fopen(REPORT_FILE, "w");
    if (report != NULL) {
        fprintf(report, "=== ОТЧЕТ: СТАТИСТИКА ПО ТИПУ КРЕСЕЛ ===\n\n");
    }
    
    for (int i = 0; i < typeCount; i++) {
        cout << "\nТип кресел: " << uniqueTypes[i] << endl;
        cout << "----------------------------------------" << endl;
        
        if (report != NULL) {
            fprintf(report, "Тип кресел: %s\n", uniqueTypes[i]);
            fprintf(report, "----------------------------------------\n");
        }
        
        struct Ticket typeTickets[MAX_TICKETS];
        int typeTicketCount = 0;
        float sumPrice = 0;
        
        for (int j = 0; j < count; j++) {
            if (strcmp(allTickets[j].chair_type, uniqueTypes[i]) == 0) {
                typeTickets[typeTicketCount] = allTickets[j];
                sumPrice += allTickets[j].price;
                typeTicketCount++;
            }
        }
        
        for (int j = 0; j < typeTicketCount - 1; j++) {
            for (int k = 0; k < typeTicketCount - j - 1; k++) {
                if (typeTickets[k].free_seats < typeTickets[k + 1].free_seats) {
                    struct Ticket temp = typeTickets[k];
                    typeTickets[k] = typeTickets[k + 1];
                    typeTickets[k + 1] = temp;
                }
            }
        }
        
        for (int j = 0; j < typeTicketCount; j++) {
            printTicket(typeTickets[j]);
            if (report != NULL) {
                fprintf(report, "Зона: %s, Мест: %d, Цена: %.2f, Льгота: %s\n",
                        typeTickets[j].zone, typeTickets[j].free_seats,
                        typeTickets[j].price,
                        typeTickets[j].is_discount_allowed ? "Да" : "Нет");
            }
        }
        
        float avgPrice = sumPrice / typeTicketCount;
        cout << "\nСредняя стоимость для типа '" << uniqueTypes[i] << "': " << avgPrice << endl;
        if (report != NULL) {
            fprintf(report, "\nСредняя стоимость: %.2f\n\n", avgPrice);
        }
    }
    
    if (report != NULL) {
        fclose(report);
        cout << "\nОтчет сохранен в файл " << REPORT_FILE << endl;
    }
    
    free(allTickets);
    waitForEnter();
}

void printMainMenu() {
    cout << "\n";
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
}

int main() {

    setlocale(LC_ALL, "en_US.UTF-8");

    clearScreen();
    cout << "ПРОГРАММА УЧЕТА БИЛЕТОВ" << endl;
    cout << "=======================" << endl << endl;
    
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) {
        f = fopen(FILENAME, "wb");
        if (f != NULL) {
            cout << "Создан новый файл " << FILENAME << endl;
            fclose(f);
        }
    } else {
        cout << "Найден файл " << FILENAME << endl;
        fclose(f);
    }
    
    cout << "\nНажмите Enter чтобы начать...";
    cin.get();
    
    int choice;
    do {
        clearScreen();
        printMainMenu();
        cout << "Выберите действие: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1: addTicket(); break;
            case 2: viewTickets(); break;
            case 3: deleteTicket(); break;
            case 4: {
                clearScreen();
                cout << "=== ЛИНЕЙНЫЙ ПОИСК ПО КОЛИЧЕСТВУ МЕСТ ===\n\n";
                int seats;
                cout << "Введите количество мест: ";
                cin >> seats;
                cin.ignore();
                
                struct Ticket results[MAX_TICKETS];
                int count = linearSearchByFreeSeats(seats, results);
                
                if (count == 0) {
                    cout << "Билеты с " << seats << " местами не найдены." << endl;
                } else {
                    cout << "Найдено билетов: " << count << endl;
                    for (int i = 0; i < count; i++) {
                        printTicket(results[i]);
                    }
                }
                waitForEnter();
                break;
            }
            case 5: {
                clearScreen();
                cout << "=== ИНТЕРПОЛЯЦИОННЫЙ ПОИСК ПО ЦЕНЕ ===\n\n";
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    cout << "Файл не найден." << endl;
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = (struct Ticket*)malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                bubbleSortByPrice(arr, n);
                
                float price;
                cout << "Введите цену для поиска: ";
                cin >> price;
                cin.ignore();
                
                int index = interpolationSearchByPrice(price, arr, n);
                
                if (index == -1) {
                    cout << "Билет с ценой " << price << " не найден." << endl;
                } else {
                    cout << "Билет найден на позиции " << index + 1 << ":" << endl;
                    printTicket(arr[index]);
                }
                
                free(arr);
                waitForEnter();
                break;
            }
            case 6: {
                clearScreen();
                cout << "=== БЫСТРАЯ СОРТИРОВКА ПО ЗОНЕ ===\n\n";
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    cout << "Файл не найден." << endl;
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = (struct Ticket*)malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                quickSortByZone(arr, 0, n - 1);
                
                cout << "Отсортированные билеты:" << endl;
                for (int i = 0; i < n; i++) {
                    printTicket(arr[i]);
                }
                
                free(arr);
                waitForEnter();
                break;
            }
            case 7: {
                clearScreen();
                cout << "=== СОРТИРОВКА ВСТАВКАМИ ПО КОЛИЧЕСТВУ МЕСТ ===\n\n";
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    cout << "Файл не найден." << endl;
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = (struct Ticket*)malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                insertionSortByFreeSeats(arr, n);
                
                cout << "Отсортированные билеты:" << endl;
                for (int i = 0; i < n; i++) {
                    printTicket(arr[i]);
                }
                
                free(arr);
                waitForEnter();
                break;
            }
            case 8: {
                clearScreen();
                cout << "=== СОРТИРОВКА ПУЗЫРЬКОМ ПО ЦЕНЕ ===\n\n";
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    cout << "Файл не найден." << endl;
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = (struct Ticket*)malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                bubbleSortByPrice(arr, n);
                
                cout << "Отсортированные билеты:" << endl;
                for (int i = 0; i < n; i++) {
                    printTicket(arr[i]);
                }
                
                free(arr);
                waitForEnter();
                break;
            }
            case 9: findSuitableSeats(); break;
            case 10: statisticsByChairType(); break;
            case 11:
                clearScreen();
                cout << "До свидания!" << endl;
                break;
            default:
                cout << "Ошибка! Введите число от 1 до 11" << endl;
                waitForEnter();
        }
    } while(choice != 11);
    
    return 0;
}