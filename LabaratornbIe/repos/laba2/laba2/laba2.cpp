#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <numbers>

using namespace std;
const double e = exp(1.0);

//Вариант 14

int main()
{
	setlocale(LC_ALL, "RU");
	double z, a, b, f, y, x;
	string xs, fs, ys;
	short int num;
	f = 0;
	ys = "y = b*cos(a*f(x)) + sin(x/5) + a * pow(e, x)";

	while (true) {
		cout << "Введите значение z, a и b:\n";
		cin >> z >> a >> b;
		if (cin.fail()) {
			system("cls");
			cout << "Значение должно являться числом.\n";
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		else {
			break;
		}
	}

	if (z > 0) {
		x = sqrt(z); xs = "x = sqrt(z)";
	}
	else {
		x = 3 * z + 1; xs = "x = 3z+1";
	}

	while (true) {
		cout << "Выберите f(x):\n" 
			<< "1) f(x) = 2x\n" 
			<< "2) f(x) = x**2\n" 
			<< "3) f(x) = x/3\n";
		cin >> num;
		if (cin.fail() || num < 1 || num > 3) {
			system("cls");
			cout << "Значение должно являться цифрой от 1 до 3.\n";
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		else {
			break;
		}
	}

	system("cls");

	switch (num) {
	case 1: f = 2 * x; fs = "f(x) = 2x"; break;
	case 2: f = pow(x, 2); fs = "f(x) = x**2"; break;
	case 3: f = x / 3; fs = "f(x) = x/3"; break;
	}

	y = b * cos(a * f) + sin(x / 5) + a * pow(e, x);

	cout << "Вычисления проводились по формулам:\n"
		<< fs << endl 
		<< xs << endl
		<< ys << endl;

	cout << "Ответ:\n" << "y = " << y;

	return 0;
}