#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>
 
using namespace std;

//Вариант 14

double Y(double x);
double S(double x, int n);
void Out_Rez(double a, double b, double h, double eps, char mode);

int main() {
    double a, b, h, eps;

    while (true) {
        cout << "Enter A, B with step H and inaccuracy EPS:\n";
        cin >> a >> b >> h >> eps;
        if (cin.fail()) {
            cout << "Not a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if (h <= 0 || b < a) {
            cout << "Invalid values: H must be > 0 and B must be >= A.\n";
            continue;
        }
        else {
            break;
        }
    }

    char mode;
    while (true) {
        cout << "Choose output column: 'S' for S(x), 'Y' for Y(x), 'D' for |Y-S| :\n";
        cin >> mode;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        mode = toupper(mode);
        if (mode == 'S' || mode == 'Y' || mode == 'D') break;
        cout << "Invalid choice.\n";
    }

    Out_Rez(a, b, h, eps, mode);
    return 0;
}

void Out_Rez(double a, double b, double h, double eps, char mode) {
    const char* colName;
    if (mode == 'S') colName = "S(x)";
    else if (mode == 'Y') colName = "Y(x)";
    else colName = "|Y - S|";

    cout << setw(10) << "X" << " | "
        << setw(15) << "N" << " | "
        << setw(15) << colName << endl;
    int totalWidth = 10 + 3 + 15 + 3 + 15;
    cout << setfill('-') << setw(totalWidth) << "" << setfill(' ') << endl;
    for (double x = a; x <= b; x += h) {
        int n = 0;
        while (true) {
            n++;
            if (fabs(Y(x) - S(x, n)) <= eps) {
                break;
            }
        }
        double y_val = Y(x);
        double s_val = S(x, n);
        double diff = fabs(y_val - s_val);
        cout << setw(10) << x << " | "
            << setw(15) << n << " | ";
        if (mode == 'S') cout << setw(15) << s_val;
        else if (mode == 'Y') cout << setw(15) << y_val;
        else cout << setw(15) << diff;
        cout << endl;
    }
}

double Y(double x) {
    return (pow(M_E, x) - pow(M_E, (-x))) / 2.0;
}

double S(double x, int n) {
    double sum = 0.0;
    double term = x;
    for (int k = 0; k <= n; k++) {
        sum += term;
        term *= x * x / ((2 * k + 2) * (2 * k + 3));
    }
    return sum;
}
