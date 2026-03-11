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
#include <cstdlib>

using namespace std;

struct Node {
    int data;
    Node* next;
};

struct Stack {
    Node* top;
    int size;
};

void initStack(Stack& s) {
    s.top = nullptr;
    s.size = 0;
}

void push(Stack& s, int val) {
    Node* newNode = new Node{val, s.top};
    s.top = newNode;
    s.size++;
}

int pop(Stack& s) {
    if (s.top == nullptr) {
        cout << "Stack underflow\n";
        return 0;
    }
    int val = s.top->data;
    Node* temp = s.top;
    s.top = s.top->next;
    delete temp;
    s.size--;
    return val;
}

void clearScreen() {
    system("cls");
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(1000, '\n');
    cin.get();
    clearScreen();
}

void viewStack(Stack& s) {
    cout << "=== Existing Stack Elements ===\n";
    Node* current = s.top;
    int count = 1;
    while (current != nullptr) {
        cout << count << ". " << current->data << "\n";
        current = current->next;
        count++;
    }
}

void createRandomStack(Stack& s) {
    clearScreen();
    cout << "=== Stack size ===\n";
    cout << "Input stack size: \n";
    int size;
    cin >> size;
    initStack(s);
    for (int i = 0; i < size; i++) {
        push(s, rand() % 101 - 50);
    }
}

void sortByDataExchange(Stack& s) {
    if (s.top == nullptr || s.top->next == nullptr) return;
    bool swapped;
    do {
        swapped = false;
        Node* current = s.top;
        while (current->next != nullptr) {
            if (current->data > current->next->data) {
                swap(current->data, current->next->data);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

void sortByAddressRearrangement(Stack& s) {
    if (s.top == nullptr || s.top->next == nullptr) return;
    bool swapped;
    do {
        swapped = false;
        Node** current = &s.top;
        while (*current != nullptr && (*current)->next != nullptr) {
            Node* a = *current;
            Node* b = a->next;
            if (a->data > b->data) {
                a->next = b->next;
                b->next = a;
                *current = b;
                swapped = true;
            }
            current = &(*current)->next;
        }
    } while (swapped);
}

void removeEverySecond(Stack& s) {
    if (s.top == nullptr) return;
    Node* current = s.top;
    Node* prev = nullptr;
    int count = 1;
    while (current != nullptr) {
        if (count % 2 == 0) {
            if (prev == nullptr) {
                s.top = current->next;
            } else {
                prev->next = current->next;
            }
            Node* temp = current;
            current = current->next;
            delete temp;
            s.size--;
        } else {
            prev = current;
            current = current->next;
        }
        count++;
    }
}

void deleteStack(Stack& s) {
    while (s.top != nullptr) {
        pop(s);
    }
}

int main() {
    Stack s;
    initStack(s);

    while (true) {
        cout << "=== Stack-Stick ===\n";
        cout << "1. Create a random new stack\n";
        cout << "2. Add a number to stack\n";
        cout << "3. View an existing stack\n";
        cout << "4. Sorting unidirectional lists:\n";
        cout << "5. Task solution\n";
        cout << "6. Exit\n";
        cout << "Input: ";

        short int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                createRandomStack(s);
                viewStack(s);
                waitForEnter();
                break;
            case 2: {
                cout << "Enter number: ";
                int val;
                cin >> val;
                push(s, val);
                viewStack(s);
                waitForEnter();
                break;
            }
            case 3:
                viewStack(s);
                waitForEnter();
                break;
            case 4: {
                cout << "   1. Rearranging the addresses of two neighboring elements\n";
                cout << "   2. Information exchange between current and next elements\n";
                cout << "Choose sorting method (1 or 2): ";
                int method;
                cin >> method;
                if (method == 1) {
                    sortByAddressRearrangement(s);
                } else if (method == 2) {
                    sortByDataExchange(s);
                } else {
                    cout << "Invalid method\n";
                }
                viewStack(s);
                waitForEnter();
                break;
            }
            case 5:
                removeEverySecond(s);
                viewStack(s);
                waitForEnter();
                break;
            case 6: {
                cout << "Exiting...\n";
                deleteStack(s);
                return 0;
            }
            default:
                cout << "Invalid choice\n";
                waitForEnter();
        }
    }

    return 0;
}