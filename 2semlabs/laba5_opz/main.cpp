/*
Обратная польская запись.
Вариант 14: a*(b-c)/(d+e)
a=0.5, b=6.1, c=8.9, d=2.4, e=7.3
*/

#include <iostream>
#include <locale>

using namespace std;

struct StackChar {
    char info;
    StackChar* next;
};

struct StackDouble {
    double info;
    StackDouble* next;
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

void inputExpression(char expr[], int size) {
    cout << "Введите выражение без пробелов: ";
    cin >> expr;

    if (expr[0] == '\0') {
        expr[0] = 'a';
        expr[1] = '\0';
    }

    expr[size - 1] = '\0';
}

double inputDouble(const char* text) {
    double x;
    cout << text;
    while (!(cin >> x)) {
        cout << "Ошибка ввода. Введите число: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return x;
}

StackChar* InStack(StackChar* p, char in) {
    StackChar* t = new StackChar;
    t->info = in;
    t->next = p;
    return t;
}

StackChar* OutStack(StackChar* p, char* out) {
    StackChar* t = p;
    *out = p->info;
    p = p->next;
    delete t;
    return p;
}

void DelCharStack(StackChar** p) {
    char c;
    while (*p != 0) {
        *p = OutStack(*p, &c);
    }
}

StackDouble* InStackDouble(StackDouble* p, double in) {
    StackDouble* t = new StackDouble;
    t->info = in;
    t->next = p;
    return t;
}

StackDouble* OutStackDouble(StackDouble* p, double* out) {
    StackDouble* t = p;
    *out = p->info;
    p = p->next;
    delete t;
    return p;
}

void DelDoubleStack(StackDouble** p) {
    double x;
    while (*p != 0) {
        *p = OutStackDouble(*p, &x);
    }
}

int Prior(char a) {
    switch (a) {
        case '*':
        case '/': return 3;
        case '+':
        case '-': return 2;
        case '(': return 1;
    }
    return 0;
}

bool IsOperation(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool IsVariable(char c) {
    return c >= 'a' && c <= 'e';
}

void PrintExpression(char expr[]) {
    for (int i = 0; expr[i] != '\0'; i++) {
        cout << expr[i];
    }
}

void PrintValues(double value[]) {
    cout << "a = " << value[0] << endl;
    cout << "b = " << value[1] << endl;
    cout << "c = " << value[2] << endl;
    cout << "d = " << value[3] << endl;
    cout << "e = " << value[4] << endl;
}

bool ToRPN(char inStr[], char outStr[]) {
    StackChar* begin = 0;
    char ss, a;
    int k = 0;
    int j = 0;

    while (inStr[k] != '\0') {
        ss = inStr[k];

        if (ss == '(') {
            begin = InStack(begin, ss);
        }
        else if (ss == ')') {
            bool found = false;
            while (begin != 0) {
                begin = OutStack(begin, &a);
                if (a == '(') {
                    found = true;
                    break;
                }
                outStr[j] = a;
                j++;
            }
            if (!found) {
                DelCharStack(&begin);
                outStr[0] = '\0';
                return false;
            }
        }
        else if (IsVariable(ss)) {
            outStr[j] = ss;
            j++;
        }
        else if (IsOperation(ss)) {
            while (begin != 0 && Prior(begin->info) >= Prior(ss)) {
                begin = OutStack(begin, &a);
                if (a == '(') {
                    DelCharStack(&begin);
                    outStr[0] = '\0';
                    return false;
                }
                outStr[j] = a;
                j++;
            }
            begin = InStack(begin, ss);
        }
        else {
            DelCharStack(&begin);
            outStr[0] = '\0';
            return false;
        }

        k++;
    }

    while (begin != 0) {
        begin = OutStack(begin, &a);
        if (a == '(') {
            DelCharStack(&begin);
            outStr[0] = '\0';
            return false;
        }
        outStr[j] = a;
        j++;
    }

    outStr[j] = '\0';
    return true;
}

bool CalculateRPN(char outStr[], double value[], double* result) {
    StackDouble* begin = 0;
    char ch;
    double op1, op2, rez;

    for (int i = 0; outStr[i] != '\0'; i++) {
        ch = outStr[i];

        if (IsVariable(ch)) {
            begin = InStackDouble(begin, value[ch - 'a']);
        }
        else if (IsOperation(ch)) {
            if (begin == 0) {
                DelDoubleStack(&begin);
                return false;
            }
            begin = OutStackDouble(begin, &op1);

            if (begin == 0) {
                DelDoubleStack(&begin);
                return false;
            }
            begin = OutStackDouble(begin, &op2);

            switch (ch) {
                case '+': rez = op2 + op1; break;
                case '-': rez = op2 - op1; break;
                case '*': rez = op2 * op1; break;
                case '/':
                    if (op1 == 0) {
                        DelDoubleStack(&begin);
                        return false;
                    }
                    rez = op2 / op1;
                    break;
                default:
                    DelDoubleStack(&begin);
                    return false;
            }

            begin = InStackDouble(begin, rez);
        }
        else {
            DelDoubleStack(&begin);
            return false;
        }
    }

    if (begin == 0) return false;

    begin = OutStackDouble(begin, result);

    if (begin != 0) {
        DelDoubleStack(&begin);
        return false;
    }

    return true;
}

int main() {
    locale::global(locale(""));

    char expression[100] = "a*(b-c)/(d+e)";
    char rpn[100] = "";
    double value[5] = {0.5, 6.1, 8.9, 2.4, 7.3};
    double result;
    int choice;

    while (true) {
        cout << "\n========== МЕНЮ ==========" << endl;
        cout << "1. Показать исходные данные" << endl;
        cout << "2. Перевести выражение в ОПЗ" << endl;
        cout << "3. Посчитать выражение" << endl;
        cout << "4. Изменить значения a, b, c, d, e" << endl;
        cout << "5. Изменить выражение" << endl;
        cout << "0. Выход" << endl;

        choice = inputInt("Ваш выбор: ");

        switch (choice) {
            case 1:
                cout << "Выражение: ";
                PrintExpression(expression);
                cout << endl;
                PrintValues(value);
                break;

            case 2:
                if (ToRPN(expression, rpn)) {
                    cout << "ОПЗ: ";
                    PrintExpression(rpn);
                    cout << endl;
                } else {
                    cout << "Ошибка в выражении. Проверьте скобки и символы." << endl;
                }
                break;

            case 3:
                if (!ToRPN(expression, rpn)) {
                    cout << "Ошибка в выражении. Расчет невозможен." << endl;
                    break;
                }

                if (CalculateRPN(rpn, value, &result)) {
                    cout << "Выражение: ";
                    PrintExpression(expression);
                    cout << endl;
                    cout << "ОПЗ: ";
                    PrintExpression(rpn);
                    cout << endl;
                    cout << "Результат: " << result << endl;
                } else {
                    cout << "Ошибка вычисления. Возможна ошибка выражения или деление на ноль." << endl;
                }
                break;

            case 4:
                value[0] = inputDouble("a = ");
                value[1] = inputDouble("b = ");
                value[2] = inputDouble("c = ");
                value[3] = inputDouble("d = ");
                value[4] = inputDouble("e = ");
                break;

            case 5:
                cout << "Можно использовать только a, b, c, d, e, операции + - * / и скобки." << endl;
                inputExpression(expression, 100);
                rpn[0] = '\0';
                break;

            case 0:
                return 0;

            default:
                cout << "Такого пункта меню нет." << endl;
        }
    }
}
