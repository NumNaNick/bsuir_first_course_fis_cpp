#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <limits>
//#include <windows.h>
//14 Вариант

using namespace std;

double Y(double x);
double S(double x, int m);
void Out_Rez(double x, double y_val, double s_val, int mode);

int main() {
    setlocale(LC_ALL, "RU");
    //SetConsoleOutputCP(CP_UTF8);
    //SetConsoleCP(CP_UTF8);

    double a, b, h, n;
    int mode;

    cout << "Input a b h: ";
    while (!(cin >> a >> b >> h) || h <= 0 || b < a) {
        cout << "a b h (h > 0, b >= a): ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    cout << "Choose output (1-Y, 2-S, 3-|Y-S|): ";
    while (!(cin >> mode) || (mode < 1 || mode > 3)) {
        cout << "Choose 1, 2 or 3: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    n = 1;
    bool accuracy = false;
    int maxN = 1000;

    for (; n <= maxN; n++) {
        accuracy = true;
        for (double x = a; x <= b; x += h) {
            if (fabs(Y(x) - S(x, n)) >= 0.001) {
                accuracy = false;
                break;
            }
        }
        if (accuracy) break;
    }

    for (double x = a; x <= b; x += h) {
        double y_val = Y(x);
        double s_val = S(x, n);
        cout << "x = " << x << "  ";
        Out_Rez(x, y_val, s_val, mode);
        cout << "  (Y=" << y_val << "  S=" << s_val << ")\n";
        if (h == 0) break;
    }
    cout << "Amount of N: "  << n;

    return 0;
}

double Y(double x) {
    return (pow(M_E, x) - pow(M_E, -x)) / 2.0;
}

double S(double x, int m) {
    double s = 0.0;
    double term = x; // Если k=0, то x^(1)/1! = x
    s += term;
    for (int k = 1; k <= m; ++k) {
        double denom = (2.0 * k) * (2.0 * k + 1.0);
        term *= (x * x) / denom;
        s += term;
        // Защита от накопления
        if (!isfinite(term)) break;
    }
    return s;
}

void Out_Rez(double x, double y_val, double s_val, int mode) {
    switch (mode) {
    case 1:
        cout << "Y(x) = " << y_val;
        break;
    case 2:
        cout << "S(x) = " << s_val;
        break;
    case 3:
        cout << "|Y(x) - S(x)| = " << fabs(y_val - s_val);
        break;
    default:
        cout << "Wrong numder.";
    }
}