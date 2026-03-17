/*
14. Имеется расписание движения междугородных автобусов. Каждый эле-
мент расписания содержит следующую информацию: номер рейса, время от-
правления, пункт назначения, время прибытия в пункт назначения. Вывести ин-
формацию о рейсах до города Могилев в порядке возрастания времени их от-
правления.

СОРТИРОВКА И ПОИСК В МАССИВАХ
14. Ключ: номер рейса автобуса. Методы сортировки: QuickSort и метод Шелла.
14. Вывести на экран пункт назначения автобуса с номером рейса 295. 
Методы поиска: линейный с барьером и двоичный.
*/

#include <stdio.h>
#include <iostream>

#define FILENAME "tickets.dat"
#define MAX_TICKETS 1024
#define MAX_ARRIVAL_LEN 64

using namespace std;

struct Ticket {
  int number;
  int beginHrs;
  int beginMin;
  char arrivalPoint[MAX_ARRIVAL_LEN];
  int endHrs;
  int endMin;
};

void clearScreen() {
  system("cls");
}

void waitForEnter() {
  cout << "\nPress Enter to continue...";
  cin.ignore(1000, '\n');
  cin.get();
  clearScreen();
}

int loadTickets(Ticket tickets[]) {
  int count = 0;
  FILE *in = fopen(FILENAME, "r");
  if (!in)
    return 0;

  while (count < MAX_TICKETS &&
         fscanf(in, "%d %d %d %s %d %d", &tickets[count].number,
                &tickets[count].beginHrs, &tickets[count].beginMin,
                tickets[count].arrivalPoint, &tickets[count].endHrs,
                &tickets[count].endMin) == 6) {
    count++;
  }

  fclose(in);
  return count;
}

void saveTickets(const Ticket tickets[], int count) {
  FILE *out = fopen(FILENAME, "w");
  if (!out)
    return;

  for (int i = 0; i < count; i++) {
    fprintf(out, "%d %d %d %s %d %d\n", tickets[i].number, tickets[i].beginHrs,
            tickets[i].beginMin, tickets[i].arrivalPoint, tickets[i].endHrs,
            tickets[i].endMin);
  }
  fclose(out);
}

void printTicket(const Ticket &t) {
  cout << "----------------------------------------\n";
  cout << "Ticket Number: " << t.number << "\n";
  cout << "Departure Time: " << (t.beginHrs < 10 ? "0" : "") << t.beginHrs
       << ":" << (t.beginMin < 10 ? "0" : "") << t.beginMin << "\n";
  cout << "Arrival Point: " << t.arrivalPoint << "\n";
  cout << "Arrival Time: " << (t.endHrs < 10 ? "0" : "") << t.endHrs
       << ":" << (t.endMin < 10 ? "0" : "") << t.endMin << "\n";
}

void addTicket(Ticket tickets[], int &count) {
  if (count >= MAX_TICKETS) {
    cout << "Ticket storage full." << endl;
    waitForEnter();
    return;
  }

  Ticket t;
  cout << "Enter ticket number: ";
  cin >> t.number;

  cout << "Enter departure time (HH MM): ";
  cin >> t.beginHrs >> t.beginMin;

  cout << "Enter arrival point (no spaces): ";
  cin >> t.arrivalPoint;

  cout << "Enter arrival time (HH MM): ";
  cin >> t.endHrs >> t.endMin;

  tickets[count++] = t;
  saveTickets(tickets, count);
  cout << "Ticket added successfully." << endl;
  waitForEnter();
}

void viewTickets(const Ticket tickets[], int count) {
  if (count == 0) {
    cout << "No tickets available." << endl;
    waitForEnter();
    return;
  }

  cout << "All tickets:\n";
  for (int i = 0; i < count; i++) {
    printTicket(tickets[i]);
  }
  waitForEnter();
}

void deleteTicket(Ticket tickets[], int &count) {
  if (count == 0) {
    cout << "No tickets to delete." << endl;
    waitForEnter();
    return;
  }

  int number;
  cout << "Enter ticket number to delete: ";
  cin >> number;

  int newCount = 0;
  for (int i = 0; i < count; i++) {
    if (tickets[i].number != number) {
      tickets[newCount++] = tickets[i];
    }
  }

  if (newCount == count) {
    cout << "Ticket not found." << endl;
  } else {
    count = newCount;
    saveTickets(tickets, count);
    cout << "Ticket deleted." << endl;
  }

  waitForEnter();
}

// Sorting by ticket number
void quickSort(Ticket a[], int left, int right) {
  if (left >= right)
    return;
  int i = left;
  int j = right;
  Ticket pivot = a[(left + right) / 2];

  while (i <= j) {
    while (a[i].number < pivot.number)
      i++;
    while (a[j].number > pivot.number)
      j--;
    if (i <= j) {
      Ticket tmp = a[i];
      a[i] = a[j];
      a[j] = tmp;
      i++;
      j--;
    }
  }

  if (left < j)
    quickSort(a, left, j);
  if (i < right)
    quickSort(a, i, right);
}

void shellSort(Ticket a[], int n) {
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      Ticket temp = a[i];
      int j;
      for (j = i; j >= gap && a[j - gap].number > temp.number; j -= gap)
        a[j] = a[j - gap];
      a[j] = temp;
    }
  }
}

// Linear search with barrier
int linearSearchBarrier(Ticket a[], int n, int key) {
  if (n <= 0)
    return -1;

  Ticket last = a[n - 1];
  a[n - 1].number = key;

  int i = 0;
  while (a[i].number != key)
    i++;

  a[n - 1] = last;

  if (i == n - 1 && last.number != key)
    return -1;
  return i;
}

// Binary search
int binarySearch(const Ticket a[], int n, int key) {
  int left = 0;
  int right = n - 1;
  while (left <= right) {
    int mid = (left + right) / 2;
    if (a[mid].number == key)
      return mid;
    if (a[mid].number < key)
      left = mid + 1;
    else
      right = mid - 1;
  }
  return -1;
}

void showRoute295(const Ticket tickets[], int count) {
  if (count == 0) {
    cout << "No tickets available." << endl;
    waitForEnter();
    return;
  }

  cout << "Choose the search method:\n";
  cout << "1. Linear search (with barrier)\n";
  cout << "2. Binary search (requires sorted by number)\n";
  int method;
  cin >> method;

  int index = -1;
  const int key = 295;
  Ticket found;
  bool foundFlag = false;

  if (method == 1) {
    Ticket copy[MAX_TICKETS];
    for (int i = 0; i < count; i++)
      copy[i] = tickets[i];
    index = linearSearchBarrier(copy, count, key);
    if (index != -1) {
      found = copy[index];
      foundFlag = true;
    }
  } else if (method == 2) {
    Ticket copy[MAX_TICKETS];
    for (int i = 0; i < count; i++)
      copy[i] = tickets[i];
    quickSort(copy, 0, count - 1);
    index = binarySearch(copy, count, key);
    if (index != -1) {
      found = copy[index];
      foundFlag = true;
    }
  } else {
    cout << "Invalid choice." << endl;
    waitForEnter();
    return;
  }

  if (!foundFlag) {
    cout << "Ticket 295 not found." << endl;
  } else {
    cout << "Ticket 295 arrival point: " << found.arrivalPoint << "\n";
  }

  waitForEnter();
}

int main() {
  Ticket tickets[MAX_TICKETS];
  int count = loadTickets(tickets);

  while (true) {
    cout << "=================================\n";
    cout << "         TICKET SCHEDULE\n";
    cout << "=================================\n";
    cout << "1. Add a ticket\n";
    cout << "2. View tickets\n";
    cout << "3. Delete a ticket\n";
    cout << "4. QuickSort (by ticket number)\n";
    cout << "5. Shell Sort (by ticket number)\n";
    cout << "6. Route 295 (search)\n";
    cout << "7. Exit\n";

    cout << "Input: ";
    int choice;
    cin >> choice;

    switch (choice) {
      case 1:
        addTicket(tickets, count);
        break;
      case 2:
        viewTickets(tickets, count);
        break;
      case 3:
        deleteTicket(tickets, count);
        break;
      case 4:
        if (count > 0) {
          quickSort(tickets, 0, count - 1);
          saveTickets(tickets, count);
          cout << "Tickets sorted by ticket number (QuickSort)." << endl;
        } else {
          cout << "No tickets to sort." << endl;
        }
        waitForEnter();
        break;
      case 5:
        if (count > 0) {
          shellSort(tickets, count);
          saveTickets(tickets, count);
          cout << "Tickets sorted by ticket number (Shell Sort)." << endl;
        } else {
          cout << "No tickets to sort." << endl;
        }
        waitForEnter();
        break;
      case 6:
        showRoute295(tickets, count);
        break;
      case 7:
        return 0;
      default:
        cout << "Invalid choice." << endl;
        waitForEnter();
        break;
    }
  }

  return 0;
}
