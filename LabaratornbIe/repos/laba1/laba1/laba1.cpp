#include <iostream>
#include <math.h>
#include <stdlib.h>

//Вариант 14

int main()
{
	setlocale(LC_ALL, "RU");
	double a, b, z01, z02;

	while (true) {
		std::cout << "Введите значение a и b:\n";
		std::cin >> a >> b;
		if (std::cin.fail()) {
			system("cls");
			std::cout << "Значение должно являться числом.\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			continue;
		}
		if (a <= 0 || b < 0 || sqrt(a * a * a * b) + a * b + a * a - a == 0)
		{
			system("cls");
			std::cout << "Делить на ноль нельзя.\n";
			continue;
		}
		else {
			break;
		}
	}

	z01 = 1 - pow(sin(2 * a), 2) / 4 + cos(2 * a);
	z02 = pow(cos(a), 2) + pow(cos(a), 4);

	std::cout << "\nОтвет:\n" << "z1 = " << z01 << "\nz2 = " << z02;

	return 0;
}