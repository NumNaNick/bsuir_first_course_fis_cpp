/*
Написать программу по созданию, добавлению, просмотру и решению поставленной задачи
для однонаправленного линейного списка типа СТЕК. 
Реализовать сортировку стека двумя рассмотренными выше методами.
Решение поставленной задачи представить в виде блок-схемы.
Во всех заданиях создать список из положительных и отрицательных случайных целых чисел.
*/
//ВАРИАНТ 14 
//Из созданного списка удалить каждый второй элемент
#include <iostream>

using namespace std;

struct Stack {
    int info;
    Stack* next;
};

Stack* InStack(Stack* p, int in) {
    Stack* t = new Stack;
    t->info = in;
    t->next = p;
    return t;
}

Stack* OutStack(Stack* p, int* out) {
    Stack* t = p;
    *out = p->info;
    p = p->next;
    delete t;
    return p;
}

void View(Stack* p) {
    Stack* t = p;
    while (t != NULL) {
        cout << t->info << " ";
        t = t->next;
    }
    cout << endl;
}

void Del_All(Stack** p) {
    Stack* t;
    while (*p != NULL) {
        t = *p;
        *p = (*p)->next;
        delete t;
    }
}

void sortByDataExchange(Stack* p) {
    if (p == NULL) return;
    Stack *t = NULL, *t1;
    int r;
    do {
        t1 = p;
        while (t1->next != t) {
            if (t1->info > t1->next->info) {
                r = t1->info;
                t1->info = t1->next->info;
                t1->next->info = r;
            }
            t1 = t1->next;
        }
        t = t1;
    } while (p->next != t);
}

void sortByAddressRearrangement(Stack** p) {
    if (*p == NULL || (*p)->next == NULL) return;

    *p = InStack(*p, 0); 

    Stack *t, *t1, *r;
    bool swapped;
    do {
        swapped = false;
        t = *p; 
        while (t->next != NULL && t->next->next != NULL) {
            t1 = t->next;
            r = t1->next;
            if (t1->info > r->info) {
                t1->next = r->next;
                r->next = t1;
                t->next = r;
                swapped = true;
            }
            t = t->next;
        }
    } while (swapped);

    int temp;
    *p = OutStack(*p, &temp);
}

void removeEverySecond(Stack** p) {
    if (*p == NULL || (*p)->next == NULL) return;
    Stack *current = *p;
    Stack *temp;
    while (current != NULL && current->next != NULL) {
        temp = current->next;
        current->next = temp->next;
        delete temp;
        current = current->next;
    }
}

int main() {
    Stack* begin = NULL; 
    int choice, n;

    while (true) {
        cout << "\n1. Create/Add Random\n2. View\n3. Sort (Addresses)\n4. Sort (Data)\n5. Task (Remove every 2nd)\n6. Exit\nChoice: ";
        while (!(cin >> choice) || choice > 6 || choice < 1) {
        cout << "Input should be numbers from 1 to 6, try again: ";
        cin.clear();
        cin.ignore(1000, '\n');
        }
        if (choice == 6) break;

        switch (choice) {
            case 1:
                system("cls");
                cout << "Count: ";
                while (!(cin >> n)) {
                    cout << "Input should be integer type, try again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    }
                for (int i = 0; i < n; i++) begin = InStack(begin, rand() % 101 - 50);
                break;
            case 2:
                system("cls"); 
                View(begin);
                break;
            case 3:
                system("cls");
                sortByAddressRearrangement(&begin);
                break;
            case 4:
                system("cls");
                sortByDataExchange(begin);
                break;
            case 5:
                system("cls");
                View(begin); cout << endl;
                removeEverySecond(&begin);
                View(begin);
                break;
        }
    }
    Del_All(&begin);
    return 0;
}