#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <string>
//14 Вариант

using namespace std;

void Out_Rez(double x, double outVal, int n);

int main() {

    double a, b, h, eps;

    cout << "Enter A, B with step H and inaccuracy EPS: ";

    while (!(cin >> a >> b >> h >> eps) || h <= 0 || b < a) {
        cout << "a b h (h > 0, b >= a): ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    int choice = 0;
    cout << "Choose output (1 - Y(x), 2 - S(x), 3 - |Y(x)-S(x)|): ";
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Enter 1, 2 or 3: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    string colName;
    if (choice == 1) colName = "Y(x)";
    else if (choice == 2) colName = "S(x)";
    else colName = "|Y(x)-S(x)|";

    cout << "X\t\t | " << colName << "\t\t | N" << endl;
    cout << "------------------------------------------------" << endl;

    for (double x = a; x <= b; x += h) {
        if (h == 0) break;

        int n = 0;
        double y_val = (exp(x) - exp(-x)) / 2.0;
        double s_val = 0.0;

        while (true) {
            n++;
            s_val = 0.0;
            double term = x; // k=0 -> x^(1)/1! = x
            s_val += term;

            for (int k = 1; k <= n; ++k) {
                double denom = (2.0 * k) * (2.0 * k + 1.0);
                term *= (x * x) / denom;
                s_val += term;
            }

            if (fabs(y_val - s_val) <= eps) {
                break;
            }
        }

        double diff = fabs(y_val - s_val);
        double outVal = 0.0;
        if (choice == 1) outVal = y_val;
        else if (choice == 2) outVal = s_val;
        else outVal = diff;

        Out_Rez(x, outVal, n);
    }

    return 0;
}

void Out_Rez(double x, double outVal, int n) {
    cout << to_string(x) << "\t |"
        << to_string(outVal) << "\t |"
        << n << endl;
}