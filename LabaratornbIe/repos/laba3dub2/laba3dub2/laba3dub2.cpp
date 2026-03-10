#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    double a, b, h;
    int n;

    cout << "Введите a: ";
    cin >> a;
    cout << "Введите b: ";
    cin >> b;
    cout << "Введите шаг h: ";
    cin >> h;
    cout << "Введите n: ";
    cin >> n;

    cout << "\n" << string(60, '-') << endl;
    cout << setw(10) << "x" << setw(15) << "S(x)" << setw(15) << "Y(x)"
        << setw(20) << "|Y(x)-S(x)|" << endl;
    cout << string(60, '-') << endl;

    cout << fixed << setprecision(6);

    for (double x = a; x <= b + 1e-6; x += h) {
        double S = 0.0;
        for (int k = 0; k <= n; k++) {
            S += pow(x, 2 * k) / factorial(2 * k);
        }

        double Y = (exp(x) + exp(-x)) / 2.0;

        cout << setw(10) << x << setw(15) << S << setw(15) << Y
            << setw(20) << fabs(Y - S) << endl;
    }

    return 0;
}