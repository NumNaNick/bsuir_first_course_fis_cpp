#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>
 
using namespace std;

//Вариант 14

double Y(double x);
double S(double x, int n);

int main() {
    double a, b, h, eps;

	while (true) {
		cout << "Enter A, B with step H and inaccuracy EPS:\n";
		cin >> a >> b >> h >> eps;
		if (cin.fail()) {
			system("cls");
			cout << "Not a number.\n";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (h <= 0 || b < a) {
			system("cls");
			cout << "Invalid values: H must be > 0 and B must be >= A.\n";
			continue;
		}
		else {
			break;
		}
	}

	cout << setw(10) << "X" << " | "
		<< setw(15) << "Y(x)" << " | "
		<< setw(15) << "S(x)" << " | "
		<< setw(15) << "|Y - S|" << " | "
		<< setw(5) << "N" << endl;
	int totalWidth = 10 + 3 + 15 + 3 + 15 + 3 + 15 + 3 + 5;
	cout << setfill('-') << setw(totalWidth) << "" << setfill(' ') << endl;
    for (double x = a; x <= b; x += h) {
		int n = 0;
		while (true) {
			
        	if (fabs(Y(x) - S(x, n)) <= eps) {
            	break;
        	}
n++;
        }	
        double y_val = Y(x);
        double s_val = S(x, n);
        double diff = fabs(y_val - s_val);
		cout << setw(10) << x << " | "
			<< setw(15) << y_val << " | "
			<< setw(15) << s_val << " | "
			<< setw(15) << diff << " | "
			<< setw(5) << n << endl;
    }
    return 0;
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