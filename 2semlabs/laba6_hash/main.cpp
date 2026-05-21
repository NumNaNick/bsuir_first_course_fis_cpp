/*
Хеширование
Вариант 14

Расписание междугородных автобусов
Поля: номер рейса, время отправления, пункт назначения, время прибытия

Задача: вывести рейсы до заданного пункта в порядке возрастания времени отправления
Сортировка по ключу "номер рейса" методами QuickSort и Шелла
Поиск автобуса с номером рейса 295 линейным поиском с барьером и двоичным поиском

Хеш-таблица по ключу "номер рейса", схема — с произвольной адресацией
*/

#include <iostream>
#include <locale>

using namespace std;

const int MAX_RECORDS = 40;
const int TEXT_SIZE = 30;
const int HASH_SIZE = 41;

struct Bus {
    int flight;
    int depart;
    char dest[TEXT_SIZE];
    int arrive; 
};

struct HashCell {
    Bus data;
    int state;
};

//Произвольная запись
const int R[HASH_SIZE] = {
    0, 17, 3, 29, 8, 35, 12, 23, 1, 39,
    6, 31, 15, 27, 4, 33, 10, 21, 37, 14,
    25, 2, 40, 7, 30, 19, 36, 11, 26, 5,
    34, 16, 24, 9, 38, 13, 28, 20, 32, 18, 22
};

int inputInt(const char* text);
int inputRange(const char* text, int left, int right);
int makeTime(int h, int m);
void printTime(int minutes);
void copyText(char to[], const char from[], int size);
bool textEqual(const char a[], const char b[]);
int positiveMod(int a, int m);

void clearArray(Bus a[], int& n);
bool addBus(Bus a[], int& n, int flight, int depart, const char dest[], int arrive);
void addBusManual(Bus a[], int& n);
void createDefaultData(Bus a[], int& n);
void deleteBusByFlight(Bus a[], int& n);
void printBus(const Bus& b);
void viewArray(const Bus a[], int n);
void copyArray(Bus to[], const Bus from[], int n);

void shellSortByFlight(Bus a[], int n);
void quickSortByFlight(Bus a[], int left, int right);
void selectionSortByDeparture(Bus a[], int n);
void showRoutesToCity(const Bus a[], int n);

int linearBarrierSearch(Bus a[], int n, int key);
int binarySearchByFlight(Bus a[], int n, int key);
void runLinearSearch(Bus a[], int n);
void runBinarySearch(const Bus a[], int n);

void initHash(HashCell h[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        h[i].state = 0;
        h[i].data.flight = 0;
        h[i].data.depart = 0;
        h[i].data.arrive = 0;
        h[i].data.dest[0] = '\0';
    }
}

bool insertHash(HashCell h[], const Bus& b) {
    int base = positiveMod(b.flight, HASH_SIZE);

    for (int p = 0; p < HASH_SIZE; p++) {
        int index = (base + R[p]) % HASH_SIZE;
        if (h[index].state == 0) {
            h[index].data = b;
            h[index].state = 1;
            return true;
        }
    }

    return false;
}

void buildHash(HashCell h[], const Bus a[], int n) {
    initHash(h);
    for (int i = 0; i < n; i++) {
        if (!insertHash(h, a[i])) {
            cout << "Не удалось вставить рейс " << a[i].flight << ": хеш-таблица заполнена.\n";
        }
    }
}

int searchHash(const HashCell h[], int key) {
    int base = positiveMod(key, HASH_SIZE);

    for (int p = 0; p < HASH_SIZE; p++) {
        int index = (base + R[p]) % HASH_SIZE;

        if (h[index].state == 0) return -1;
        if (h[index].data.flight == key) return index;
    }

    return -1;
}

void viewHash(const HashCell h[]) {
    cout << "\n--- Хеш-таблица. Ключ: номер рейса. Схема: произвольная адресация ---\n";
    for (int i = 0; i < HASH_SIZE; i++) {
        cout << "H[" << i << "] = ";
        if (h[i].state == 0) {
            cout << "empty\n";
        } else {
            printBus(h[i].data);
        }
    }
}

void runHashSearch(HashCell h[], const Bus a[], int n, bool& hashReady) {
    if (n == 0) {
        cout << "Массив пуст.\n";
        return;
    }

    if (!hashReady) {
        buildHash(h, a, n);
        hashReady = true;
        cout << "Хеш-таблица была устаревшей, поэтому пересоздана автоматически.\n";
    }

    int key = inputInt("Введите номер рейса для поиска в хеш-таблице: ");
    int pos = searchHash(h, key);

    if (pos == -1) {
        cout << "Рейс не найден в хеш-таблице.\n";
    } else {
        cout << "Найден в ячейке H[" << pos << "]:\n";
        printBus(h[pos].data);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Bus buses[MAX_RECORDS + 1];
    HashCell hashTable[HASH_SIZE];
    int n = 0;
    bool hashReady = false;

    createDefaultData(buses, n);
    initHash(hashTable);

    int choice;
    while (true) {
        cout << "\n========== МЕНЮ ==========\n";
        cout << "1. Создать исходное расписание заново\n";
        cout << "2. Добавить рейс\n";
        cout << "3. Удалить рейс по номеру\n";
        cout << "4. Показать исходный массив\n";
        cout << "5. Сортировать по номеру рейса: QuickSort\n";
        cout << "6. Сортировать по номеру рейса: метод Шелла\n";
        cout << "7. Задача: рейсы до города по времени отправления\n";
        cout << "8. Поиск номера рейса линейным поиском с барьером\n";
        cout << "9. Поиск номера рейса двоичным поиском\n";
        cout << "10. Пересоздать и показать хеш-таблицу\n";
        cout << "11. Найти рейс в хеш-таблице\n";
        cout << "0. Выход\n";

        choice = inputRange("Ваш выбор: ", 0, 11);

        if (choice == 0) break;

        switch (choice) {
        case 1:
            createDefaultData(buses, n);
            hashReady = false;
            cout << "Исходное расписание создано заново. Хеш-таблица устарела.\n";
            break;
        case 2:
            addBusManual(buses, n);
            hashReady = false;
            break;
        case 3:
            deleteBusByFlight(buses, n);
            hashReady = false;
            break;
        case 4:
            viewArray(buses, n);
            break;
        case 5:
            quickSortByFlight(buses, 0, n - 1);
            hashReady = false;
            cout << "Массив отсортирован методом QuickSort по номеру рейса.\n";
            viewArray(buses, n);
            break;
        case 6:
            shellSortByFlight(buses, n);
            hashReady = false;
            cout << "Массив отсортирован методом Шелла по номеру рейса.\n";
            viewArray(buses, n);
            break;
        case 7:
            showRoutesToCity(buses, n);
            break;
        case 8:
            runLinearSearch(buses, n);
            break;
        case 9:
            runBinarySearch(buses, n);
            break;
        case 10:
            buildHash(hashTable, buses, n);
            hashReady = true;
            cout << "Хеш-таблица пересоздана из текущего массива.\n";
            viewHash(hashTable);
            break;
        case 11:
            runHashSearch(hashTable, buses, n, hashReady);
            break;
        }
    }

    return 0;
}

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

int inputRange(const char* text, int left, int right) {
    int x = inputInt(text);
    while (x < left || x > right) {
        cout << "Значение должно быть от " << left << " до " << right << ".\n";
        x = inputInt(text);
    }
    return x;
}

int makeTime(int h, int m) {
    return h * 60 + m;
}

void printTwoDigits(int x) {
    if (x < 10) cout << '0';
    cout << x;
}

void printTime(int minutes) {
    printTwoDigits(minutes / 60);
    cout << ':';
    printTwoDigits(minutes % 60);
}

void copyText(char to[], const char from[], int size) {
    int i = 0;
    while (i < size - 1 && from[i] != '\0') {
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';
}

bool textEqual(const char a[], const char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

int positiveMod(int a, int m) {
    int r = a % m;
    if (r < 0) r += m;
    return r;
}

void clearArray(Bus a[], int& n) {
    n = 0;
}

bool existsFlight(const Bus a[], int n, int flight) {
    for (int i = 0; i < n; i++) {
        if (a[i].flight == flight) return true;
    }
    return false;
}

bool addBus(Bus a[], int& n, int flight, int depart, const char dest[], int arrive) {
    if (n >= MAX_RECORDS) {
        cout << "Массив заполнен. Добавление невозможно.\n";
        return false;
    }
    if (existsFlight(a, n, flight)) {
        cout << "Рейс с таким номером уже существует. По условию ключ должен быть уникальным.\n";
        return false;
    }

    a[n].flight = flight;
    a[n].depart = depart;
    copyText(a[n].dest, dest, TEXT_SIZE);
    a[n].arrive = arrive;
    n++;
    return true;
}

void addBusManual(Bus a[], int& n) {
    char city[TEXT_SIZE];
    int flight = inputInt("Номер рейса: ");
    int h1 = inputRange("Час отправления: ", 0, 23);
    int m1 = inputRange("Минуты отправления: ", 0, 59);
    cout << "Пункт назначения одним словом, например Mogilev: ";
    cin >> city;
    int h2 = inputRange("Час прибытия: ", 0, 23);
    int m2 = inputRange("Минуты прибытия: ", 0, 59);

    if (addBus(a, n, flight, makeTime(h1, m1), city, makeTime(h2, m2))) {
        cout << "Рейс добавлен. Теперь пересоздание хеш-таблицы даст другой результат.\n";
    }
}

void createDefaultData(Bus a[], int& n) {
    clearArray(a, n);

    addBus(a, n, 295, makeTime(7, 20),  "Mogilev", makeTime(10, 35));
    addBus(a, n, 184, makeTime(6, 45),  "Gomel",   makeTime(11, 10));
    addBus(a, n, 407, makeTime(9, 10),  "Mogilev", makeTime(12, 20));
    addBus(a, n, 126, makeTime(12, 30), "Brest",   makeTime(17, 50));
    addBus(a, n, 351, makeTime(5, 55),  "Mogilev", makeTime(9, 15));
    addBus(a, n, 219, makeTime(14, 40), "Vitebsk", makeTime(18, 25));
    addBus(a, n, 510, makeTime(16, 05), "Mogilev", makeTime(19, 05));
    addBus(a, n, 333, makeTime(10, 00), "Grodno",  makeTime(15, 30));
    addBus(a, n, 172, makeTime(8, 25),  "Mogilev", makeTime(11, 40));
    addBus(a, n, 462, makeTime(18, 15), "Brest",   makeTime(23, 10));
}

void deleteBusByFlight(Bus a[], int& n) {
    if (n == 0) {
        cout << "Массив пуст.\n";
        return;
    }

    int key = inputInt("Введите номер рейса для удаления: ");
    int pos = -1;
    for (int i = 0; i < n; i++) {
        if (a[i].flight == key) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        cout << "Рейс не найден.\n";
        return;
    }

    for (int i = pos; i < n - 1; i++) {
        a[i] = a[i + 1];
    }
    n--;
    cout << "Рейс удален. Теперь хеш-таблицу нужно пересоздать.\n";
}

void printBus(const Bus& b) {
    cout << "Рейс " << b.flight << ", отправление ";
    printTime(b.depart);
    cout << ", пункт " << b.dest << ", прибытие ";
    printTime(b.arrive);
    cout << endl;
}

void viewArray(const Bus a[], int n) {
    if (n == 0) {
        cout << "Исходный массив пуст.\n";
        return;
    }

    cout << "\n--- Исходный массив структур ---\n";
    for (int i = 0; i < n; i++) {
        cout << i << ". ";
        printBus(a[i]);
    }
}

void copyArray(Bus to[], const Bus from[], int n) {
    for (int i = 0; i < n; i++) to[i] = from[i];
}

void shellSortByFlight(Bus a[], int n) {
    int d = n / 2;
    while (d > 0) {
        for (int i = d; i < n; i++) {
            Bus temp = a[i];
            int j = i;
            while (j >= d && a[j - d].flight > temp.flight) {
                a[j] = a[j - d];
                j -= d;
            }
            a[j] = temp;
        }
        d /= 2;
    }
}

void quickSortByFlight(Bus a[], int left, int right) {
    if (left >= right) return;

    int i = left;
    int j = right;
    int x = a[(left + right) / 2].flight;

    while (i <= j) {
        while (a[i].flight < x) i++;
        while (a[j].flight > x) j--;
        if (i <= j) {
            Bus temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j) quickSortByFlight(a, left, j);
    if (i < right) quickSortByFlight(a, i, right);
}

void selectionSortByDeparture(Bus a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j].depart < a[minIndex].depart) minIndex = j;
        }
        if (minIndex != i) {
            Bus temp = a[i];
            a[i] = a[minIndex];
            a[minIndex] = temp;
        }
    }
}

void showRoutesToCity(const Bus a[], int n) {
    if (n == 0) {
        cout << "Массив пуст.\n";
        return;
    }

    char city[TEXT_SIZE];
    cout << "Введите пункт назначения одним словом, например Mogilev: ";
    cin >> city;

    Bus found[MAX_RECORDS];
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (textEqual(a[i].dest, city)) {
            found[k] = a[i];
            k++;
        }
    }

    if (k == 0) {
        cout << "Рейсов до указанного пункта нет.\n";
        return;
    }

    selectionSortByDeparture(found, k);

    cout << "\n--- Рейсы до " << city << " по возрастанию времени отправления ---\n";
    for (int i = 0; i < k; i++) printBus(found[i]);
}

int linearBarrierSearch(Bus a[], int n, int key) {
    a[n].flight = key; // барьер
    int i = 0;
    while (a[i].flight != key) i++;
    if (i == n) return -1;
    return i;
}

int binarySearchByFlight(Bus a[], int n, int key) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        if (a[mid].flight == key) return mid;
        if (a[mid].flight < key) left = mid + 1;
        else right = mid - 1;
    }

    return -1;
}

void runLinearSearch(Bus a[], int n) {
    if (n == 0) {
        cout << "Массив пуст.\n";
        return;
    }

    int key = inputInt("Введите номер рейса для поиска: ");
    int pos = linearBarrierSearch(a, n, key);

    if (pos == -1) cout << "Рейс не найден.\n";
    else {
        cout << "Найден линейным поиском с барьером:\n";
        printBus(a[pos]);
    }
}

void runBinarySearch(const Bus a[], int n) {
    if (n == 0) {
        cout << "Массив пуст.\n";
        return;
    }

    Bus temp[MAX_RECORDS];
    copyArray(temp, a, n);
    shellSortByFlight(temp, n);

    int key = inputInt("Введите номер рейса для поиска: ");
    int pos = binarySearchByFlight(temp, n, key);

    if (pos == -1) cout << "Рейс не найден.\n";
    else {
        cout << "Найден двоичным поиском в отсортированной копии массива:\n";
        printBus(temp[pos]);
    }
}

