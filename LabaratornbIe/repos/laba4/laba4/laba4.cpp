#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    int n;
    int arr[20] = {0};

    while (true) {
        cout << "Введите размер массива (от 1 до 20):\n";
        cin >> n;
        if (n <= 0 || n > 20 || cin.fail()) {
            system("cls");
            cout << "Ошибка! Размер должен быть от 1 до 20.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        else {
            break;
        }
    }

    int choice;
    cout << "Выберите способ:" << endl;
    cout << "1 - ввод с клавиатуры" << endl;
    cout << "2 - случайные числа" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;

    switch (choice) {
    case 1: 
        cout << "Введите " << n << " чисел:" << endl;
        for (int i = 0; i < n; i++) {
            cout << "Число " << (i + 1) << ": ";
            cin >> arr[i];
        }
    case 2: 
        srand(time(0));
        cout << "Случайные числа: ";
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100 - 50; // от -50 до 49
            cout << arr[i] << " ";
        }
        cout << endl;
    }
    
    cout << "Ваш массив: "; 
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    int min_index = 0;
    int min_value = abs(arr[0]);

    for (int i = 1; i < n; i++) {

        int current_abs = abs(arr[i]);
        if (current_abs <= min_value) {
            min_value = current_abs;
            min_index = i;
        }
    }

    cout << "Наименьший по модулю элемент: " << arr[min_index];
    cout << " (на позиции " << min_index + 1 << ")" << endl;

    if (min_index == n - 1) {
        cout << "После этого элемента нет других элементов!" << endl;
        return 0;
    }

    int sum = 0;
    cout << "Элементы после минимального: ";
    for (int i = min_index + 1; i < n; i++) {
        int element = abs(arr[i]);
        sum += element;
        cout << element << " ";
    }
    cout << endl;

    cout << "Сумма модулей: " << sum << endl;

    return 0;
}