#include <iostream>
#include <locale.h>
using namespace std;

struct Node {
    int key;
    char text[80];
    Node* left;
    Node* right;
};

void clearInput() {
    cin.clear();
    char c;
    do {
        cin.get(c);
    } while (c != '\n' && cin);
}

int readInt(const char* message) {
    int x;
    while (true) {
        cout << message;
        if (cin >> x) {
            clearInput();
            return x;
        }
        cout << "Ошибка: нужно ввести целое число.\n";
        clearInput();
    }
}

void readText(const char* message, char text[]) {
    while (true) {
        cout << message;
        cin.getline(text, 80);
        if (text[0] != '\0') return;
        cout << "Ошибка: строка не должна быть пустой.\n";
    }
}

void copyText(char to[], const char from[]) {
    int i = 0;
    while (from[i] != '\0' && i < 79) {
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';
}

Node* makeNode(int key, const char text[]) {
    Node* p = new Node;
    p->key = key;
    copyText(p->text, text);
    p->left = 0;
    p->right = 0;
    return p;
}

Node* findNode(Node* root, int key) {
    while (root != 0) {
        if (key == root->key) return root;
        if (key < root->key) root = root->left;
        else root = root->right;
    }
    return 0;
}

void addNode(Node*& root, int key, const char text[]) {
    if (root == 0) {
        root = makeNode(key, text);
        cout << "Запись добавлена.\n";
        return;
    }

    if (key == root->key) {
        cout << "Ошибка: такой ключ уже есть.\n";
        return;
    }

    if (key < root->key) addNode(root->left, key, text);
    else addNode(root->right, key, text);
}

void printTree(Node* root, int level) {
    if (root == 0) return;

    printTree(root->right, level + 1);

    for (int i = 0; i < level; i++) cout << "    ";
    cout << root->key << " - " << root->text << "\n";

    printTree(root->left, level + 1);
}

void printForward(Node* root) {
    if (root == 0) return;
    cout << root->key << " - " << root->text << "\n";
    printForward(root->left);
    printForward(root->right);
}

void printBackward(Node* root) {
    if (root == 0) return;
    printBackward(root->left);
    printBackward(root->right);
    cout << root->key << " - " << root->text << "\n";
}

void printSorted(Node* root) {
    if (root == 0) return;
    printSorted(root->left);
    cout << root->key << " - " << root->text << "\n";
    printSorted(root->right);
}

int countNodes(Node* root) {
    if (root == 0) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void saveToArray(Node* root, Node** a, int& i) {
    if (root == 0) return;
    saveToArray(root->left, a, i);
    a[i] = root;
    i++;
    saveToArray(root->right, a, i);
}

Node* makeBalanced(Node** a, int left, int right) {
    if (left > right) return 0;

    int middle = (left + right) / 2;
    Node* root = a[middle];
    root->left = makeBalanced(a, left, middle - 1);
    root->right = makeBalanced(a, middle + 1, right);
    return root;
}

void balanceTree(Node*& root) {
    int n = countNodes(root);
    if (n == 0) {
        cout << "Дерево пустое.\n";
        return;
    }

    Node** a = new Node*[n];
    int i = 0;
    saveToArray(root, a, i);
    root = makeBalanced(a, 0, n - 1);
    delete[] a;

    cout << "Дерево сбалансировано.\n";
}

Node* deleteNode(Node* root, int key) {
    if (root == 0) {
        cout << "Ключ не найден.\n";
        return 0;
    }

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
        return root;
    }

    if (key > root->key) {
        root->right = deleteNode(root->right, key);
        return root;
    }

    if (root->left == 0) {
        Node* temp = root->right;
        delete root;
        cout << "Запись удалена.\n";
        return temp;
    }

    if (root->right == 0) {
        Node* temp = root->left;
        delete root;
        cout << "Запись удалена.\n";
        return temp;
    }

    Node* minRight = root->right;
    while (minRight->left != 0) minRight = minRight->left;

    root->key = minRight->key;
    copyText(root->text, minRight->text);
    root->right = deleteNode(root->right, minRight->key);
    return root;
}

void deleteTree(Node*& root) {
    if (root == 0) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
    root = 0;
}

void addFromKeyboard(Node*& root) {
    int key = readInt("Введите ключ: ");
    char text[80];
    readText("Введите строку: ", text);
    addNode(root, key, text);
}

void createTree(Node*& root) {
    deleteTree(root);

    int n;
    do {
        n = readInt("Сколько записей создать: ");
        if (n <= 0) cout << "Ошибка: количество должно быть больше 0.\n";
    } while (n <= 0);

    for (int i = 0; i < n; i++) {
        cout << "\nЗапись " << i + 1 << "\n";
        addFromKeyboard(root);
    }
}

void searchByKey(Node* root) {
    int key = readInt("Введите ключ для поиска: ");
    Node* p = findNode(root, key);

    if (p == 0) cout << "Запись не найдена.\n";
    else cout << "Найдена запись: " << p->key << " - " << p->text << "\n";
}

void leftBranchTask(Node* root) {
    if (root == 0) {
        cout << "Дерево пустое.\n";
        return;
    }

    cout << "Количество записей в левой ветви дерева: "
         << countNodes(root->left) << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    Node* root = 0;
    int menu;

    do {
        cout << "------------ МЕНЮ -------------";
        cout << "\n1 - Создать дерево\n";
        cout << "2 - Добавить запись\n";
        cout << "3 - Показать дерево\n";
        cout << "4 - Найти по ключу\n";
        cout << "5 - Удалить по ключу\n";
        cout << "6 - Сбалансировать дерево\n";
        cout << "7 - Прямой обход\n";
        cout << "8 - Обратный обход\n";
        cout << "9 - По возрастанию ключа\n";
        cout << "10 - Пункт 14: записи в левой ветви\n";
        cout << "0 - Выход\n";

        menu = readInt("Ваш выбор: ");

        switch (menu) {
            case 1:
                system("clear");
                createTree(root);
                break;
            case 2:
                system("clear");
                addFromKeyboard(root);
                break;
            case 3:
                system("clear");
                if (root == 0) cout << "Дерево пустое.\n";
                else printTree(root, 0);
                break;
            case 4:
                system("clear");
                searchByKey(root);
                break;
            case 5: {
                    system("clear");
                int key = readInt("Введите ключ для удаления: ");
                root = deleteNode(root, key);
                break;
            }
            case 6:
                system("clear");
                balanceTree(root);
                break;
            case 7:
                system("clear");
                if (root == 0) cout << "Дерево пустое.\n";
                else printForward(root);
                break;
            case 8:
                system("clear");
                if (root == 0) cout << "Дерево пустое.\n";
                else printBackward(root);
                break;
            case 9:
                system("clear");
                if (root == 0) cout << "Дерево пустое.\n";
                else printSorted(root);
                break;
            case 10:
                system("clear");
                leftBranchTask(root);
                break;
            case 0:
                system("clear");
                deleteTree(root);
                cout << "Выход.\n";
                break;
            default:
                system("clear");
                cout << "Ошибка: такого пункта меню нет.\n";
        }
    } while (menu != 0);

    return 0;
}
