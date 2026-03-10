#include <stdio.h>
#include <stdlib.h>

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

void clearScreen() {
    system("cls");
}

void waitForEnter() {
    printf("\nНажмите Enter чтобы продолжить...");
    fflush(stdin);
    getchar();
}

int getFileSize() {
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) return 0;
    fseek(f, 0, SEEK_END);
    int size = ftell(f) / sizeof(struct Ticket);
    fclose(f);
    return size;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printTicket(struct Ticket t) {
    printf("----------------------------------------\n");
    printf("Зона: %s\n", t.zone);
    printf("Свободных мест: %d\n", t.free_seats);
    printf("Стоимость: %.2f\n", t.price);
    printf("Тип кресел: %s\n", t.chair_type);
    printf("Льготный билет: %s\n", t.is_discount_allowed ? "Да" : "Нет");
}

void addTicket() {
    clearScreen();
    struct Ticket newTicket;
    FILE *f;
    
    printf("\n=== ДОБАВЛЕНИЕ НОВОГО БИЛЕТА ===\n\n");
    
    clearInputBuffer();
    
    printf("Введите зону расположения: ");
    fgets(newTicket.zone, 50, stdin);
    for (int i = 0; i < 50; i++) {
        if (newTicket.zone[i] == '\n') {
            newTicket.zone[i] = '\0';
            break;
        }
    }
    
    printf("Введите количество свободных мест: ");
    scanf("%d", &newTicket.free_seats);
    clearInputBuffer();
    
    printf("Введите стоимость билета: ");
    scanf("%f", &newTicket.price);
    clearInputBuffer();
    
    printf("Введите тип кресел: ");
    fgets(newTicket.chair_type, 30, stdin);
    for (int i = 0; i < 30; i++) {
        if (newTicket.chair_type[i] == '\n') {
            newTicket.chair_type[i] = '\0';
            break;
        }
    }
    
    printf("Разрешена ли льгота? (1-да, 0-нет): ");
    scanf("%d", &newTicket.is_discount_allowed);
    clearInputBuffer();
    
    f = fopen(FILENAME, "ab");
    if (f == NULL) {
        printf("Ошибка открытия файла!\n");
        return;
    }
    
    fwrite(&newTicket, sizeof(struct Ticket), 1, f);
    fclose(f);
    
    printf("\nБилет успешно добавлен!\n");
    waitForEnter();
}

void viewTickets() {
    clearScreen();
    struct Ticket t;
    FILE *f;
    int count = 0;
    
    printf("\n=== ВСЕ БИЛЕТЫ ===\n\n");
    
    f = fopen(FILENAME, "rb");
    if (f == NULL) {
        printf("Файл с билетами не найден.\n");
        waitForEnter();
        return;
    }
    
    while (fread(&t, sizeof(struct Ticket), 1, f) == 1) {
        count++;
        printTicket(t);
    }
    
    fclose(f);
    
    if (count == 0) {
        printf("Билетов пока нет.\n");
    } else {
        printf("----------------------------------------\n");
        printf("Всего билетов: %d\n", count);
    }
    
    waitForEnter();
}

void deleteTicket() {
    clearScreen();
    struct Ticket t;
    FILE *f, *temp;
    int choice, index;
    
    printf("\n=== УДАЛЕНИЕ БИЛЕТА ===\n\n");
    
    f = fopen(FILENAME, "rb");
    if (f == NULL) {
        printf("Файл с билетами не найден.\n");
        waitForEnter();
        return;
    }
    
    // Загружаем все билеты в массив
    struct Ticket *allTickets = malloc(MAX_TICKETS * sizeof(struct Ticket));
    int count = 0;
    
    printf("Доступные зоны для удаления:\n");
    printf("----------------------------------------\n");
    
    while (fread(&t, sizeof(struct Ticket), 1, f) == 1) {
        allTickets[count] = t;
        printf("%d. Зона: %s | Свободных мест: %d | Цена: %.2f | Тип: %s\n", 
               count + 1, t.zone, t.free_seats, t.price, t.chair_type);
        count++;
    }
    fclose(f);
    
    if (count == 0) {
        printf("Нет билетов для удаления.\n");
        free(allTickets);
        waitForEnter();
        return;
    }
    
    printf("----------------------------------------\n");
    printf("Выберите номер зоны для удаления (0 - отмена): ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    if (choice == 0 || choice > count) {
        printf("Удаление отменено.\n");
        free(allTickets);
        waitForEnter();
        return;
    }
    
    index = choice - 1;
    
    printf("\nВы выбрали:\n");
    printf("Зона: %s\n", allTickets[index].zone);
    printf("Свободных мест: %d\n", allTickets[index].free_seats);
    printf("Цена: %.2f\n", allTickets[index].price);
    printf("Тип кресел: %s\n", allTickets[index].chair_type);
    printf("Льготный: %s\n", allTickets[index].is_discount_allowed ? "Да" : "Нет");
    
    printf("\nТочно удалить? (1-да, 0-нет): ");
    int confirm;
    scanf("%d", &confirm);
    clearInputBuffer();
    
    if (confirm != 1) {
        printf("Удаление отменено.\n");
        free(allTickets);
        waitForEnter();
        return;
    }
    
    // Создаем новый файл без выбранного билета
    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Ошибка создания временного файла.\n");
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
    
    printf("\nБилет успешно удален!\n");
    
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
            while (1) {
                int compare = 0, k = 0;
                while (arr[i].zone[k] != '\0' && pivot.zone[k] != '\0' && arr[i].zone[k] == pivot.zone[k]) {
                    k++;
                }
                if (arr[i].zone[k] == '\0' && pivot.zone[k] == '\0') compare = 0;
                else if (arr[i].zone[k] == '\0') compare = -1;
                else if (pivot.zone[k] == '\0') compare = 1;
                else compare = arr[i].zone[k] - pivot.zone[k];
                
                if (compare < 0) i++;
                else break;
            }
            
            while (1) {
                int compare = 0, k = 0;
                while (arr[j].zone[k] != '\0' && pivot.zone[k] != '\0' && arr[j].zone[k] == pivot.zone[k]) {
                    k++;
                }
                if (arr[j].zone[k] == '\0' && pivot.zone[k] == '\0') compare = 0;
                else if (arr[j].zone[k] == '\0') compare = -1;
                else if (pivot.zone[k] == '\0') compare = 1;
                else compare = arr[j].zone[k] - pivot.zone[k];
                
                if (compare > 0) j--;
                else break;
            }
            
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
    printf("\n=== ПОИСК ПОДХОДЯЩЕГО МЕСТА ===\n\n");
    
    int neededSeats, needDiscount;
    printf("Введите необходимое количество мест: ");
    scanf("%d", &neededSeats);
    printf("Нужен ли льготный билет? (1-да, 0-нет): ");
    scanf("%d", &needDiscount);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) {
        printf("Файл с билетами не найден.\n");
        waitForEnter();
        return;
    }
    
    struct Ticket *allTickets = malloc(MAX_TICKETS * sizeof(struct Ticket));
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
        printf("\nНет подходящих мест.\n");
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
    
    printf("\nПодходящие места (в порядке возрастания цены):\n");
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
        printf("\nОтчет сохранен в файл %s\n", REPORT_FILE);
    }
    
    free(allTickets);
    waitForEnter();
}

void statisticsByChairType() {
    clearScreen();
    printf("\n=== СТАТИСТИКА ПО ТИПУ КРЕСЕЛ ===\n\n");
    
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) {
        printf("Файл с билетами не найден.\n");
        waitForEnter();
        return;
    }
    
    struct Ticket *allTickets = malloc(MAX_TICKETS * sizeof(struct Ticket));
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
            int match = 1;
            for (int k = 0; k < 30; k++) {
                if (allTickets[i].chair_type[k] != uniqueTypes[j][k]) {
                    match = 0;
                    break;
                }
                if (allTickets[i].chair_type[k] == '\0') break;
            }
            if (match) {
                found = 1;
                break;
            }
        }
        if (!found) {
            for (int k = 0; k < 30; k++) {
                uniqueTypes[typeCount][k] = allTickets[i].chair_type[k];
                if (allTickets[i].chair_type[k] == '\0') break;
            }
            typeCount++;
        }
    }
    
    FILE *report = fopen(REPORT_FILE, "w");
    if (report != NULL) {
        fprintf(report, "=== ОТЧЕТ: СТАТИСТИКА ПО ТИПУ КРЕСЕЛ ===\n\n");
    }
    
    for (int i = 0; i < typeCount; i++) {
        printf("\nТип кресел: %s\n", uniqueTypes[i]);
        printf("----------------------------------------\n");
        
        if (report != NULL) {
            fprintf(report, "Тип кресел: %s\n", uniqueTypes[i]);
            fprintf(report, "----------------------------------------\n");
        }
        
        struct Ticket typeTickets[MAX_TICKETS];
        int typeTicketCount = 0;
        float sumPrice = 0;
        
        for (int j = 0; j < count; j++) {
            int match = 1;
            for (int k = 0; k < 30; k++) {
                if (allTickets[j].chair_type[k] != uniqueTypes[i][k]) {
                    match = 0;
                    break;
                }
                if (allTickets[j].chair_type[k] == '\0') break;
            }
            if (match) {
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
        printf("\nСредняя стоимость для типа '%s': %.2f\n", uniqueTypes[i], avgPrice);
        if (report != NULL) {
            fprintf(report, "\nСредняя стоимость: %.2f\n\n", avgPrice);
        }
    }
    
    if (report != NULL) {
        fclose(report);
        printf("\nОтчет сохранен в файл %s\n", REPORT_FILE);
    }
    
    free(allTickets);
    waitForEnter();
}

void printMainMenu() {
    printf("\n");
    printf("========================================\n");
    printf("    УЧЕТ БИЛЕТОВ НА КОНЦЕРТ\n");
    printf("========================================\n");
    printf("1. Добавить билет\n");
    printf("2. Показать все билеты\n");
    printf("3. Удалить билет\n");
    printf("4. Линейный поиск по кол-ву мест\n");
    printf("5. Интерполяционный поиск по цене\n");
    printf("6. Быстрая сортировка по зоне\n");
    printf("7. Сортировка вставками по кол-ву мест\n");
    printf("8. Сортировка пузырьком по цене\n");
    printf("9. Поиск подходящего места\n");
    printf("10. Статистика по типу кресел\n");
    printf("11. Выход\n");
    printf("========================================\n");
}

int main() {

    system("chcp 65001 > nul"); //поддержка юникода для командной строки виндовс

    clearScreen();
    printf("ПРОГРАММА УЧЕТА БИЛЕТОВ\n");
    printf("=======================\n\n");
    
    FILE *f = fopen(FILENAME, "rb");
    if (f == NULL) {
        f = fopen(FILENAME, "wb");
        if (f != NULL) {
            printf("Создан новый файл %s\n", FILENAME);
            fclose(f);
        }
    } else {
        printf("Найден файл %s\n", FILENAME);
        fclose(f);
    }
    
    printf("\nНажмите Enter чтобы начать...");
    getchar();
    
    int choice;
    do {
        clearScreen();
        printMainMenu();
        printf("Выберите действие: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addTicket(); break;
            case 2: viewTickets(); break;
            case 3: deleteTicket(); break;
            case 4: {
                clearScreen();
                printf("=== ЛИНЕЙНЫЙ ПОИСК ПО КОЛИЧЕСТВУ МЕСТ ===\n\n");
                int seats;
                printf("Введите количество мест: ");
                scanf("%d", &seats);
                clearInputBuffer();
                
                struct Ticket results[MAX_TICKETS];
                int count = linearSearchByFreeSeats(seats, results);
                
                if (count == 0) {
                    printf("Билеты с %d местами не найдены.\n", seats);
                } else {
                    printf("Найдено билетов: %d\n", count);
                    for (int i = 0; i < count; i++) {
                        printTicket(results[i]);
                    }
                }
                waitForEnter();
                break;
            }
            case 5: {
                clearScreen();
                printf("=== ИНТЕРПОЛЯЦИОННЫЙ ПОИСК ПО ЦЕНЕ ===\n\n");
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    printf("Файл не найден.\n");
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                bubbleSortByPrice(arr, n);
                
                float price;
                printf("Введите цену для поиска: ");
                scanf("%f", &price);
                clearInputBuffer();
                
                int index = interpolationSearchByPrice(price, arr, n);
                
                if (index == -1) {
                    printf("Билет с ценой %.2f не найден.\n", price);
                } else {
                    printf("Билет найден на позиции %d:\n", index + 1);
                    printTicket(arr[index]);
                }
                
                free(arr);
                waitForEnter();
                break;
            }
            case 6: {
                clearScreen();
                printf("=== БЫСТРАЯ СОРТИРОВКА ПО ЗОНЕ ===\n\n");
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    printf("Файл не найден.\n");
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                quickSortByZone(arr, 0, n - 1);
                
                printf("Отсортированные билеты:\n");
                for (int i = 0; i < n; i++) {
                    printTicket(arr[i]);
                }
                
                free(arr);
                waitForEnter();
                break;
            }
            case 7: {
                clearScreen();
                printf("=== СОРТИРОВКА ВСТАВКАМИ ПО КОЛИЧЕСТВУ МЕСТ ===\n\n");
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    printf("Файл не найден.\n");
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                insertionSortByFreeSeats(arr, n);
                
                printf("Отсортированные билеты:\n");
                for (int i = 0; i < n; i++) {
                    printTicket(arr[i]);
                }
                
                free(arr);
                waitForEnter();
                break;
            }
            case 8: {
                clearScreen();
                printf("=== СОРТИРОВКА ПУЗЫРЬКОМ ПО ЦЕНЕ ===\n\n");
                
                FILE *f = fopen(FILENAME, "rb");
                if (f == NULL) {
                    printf("Файл не найден.\n");
                    waitForEnter();
                    break;
                }
                
                struct Ticket *arr = malloc(MAX_TICKETS * sizeof(struct Ticket));
                int n = 0;
                while (fread(&arr[n], sizeof(struct Ticket), 1, f) == 1) n++;
                fclose(f);
                
                bubbleSortByPrice(arr, n);
                
                printf("Отсортированные билеты:\n");
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
                printf("До свидания!\n");
                break;
            default:
                printf("Ошибка! Введите число от 1 до 11\n");
                waitForEnter();
        }
    } while(choice != 11);
    
    return 0;
}