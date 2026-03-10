// ВАРИАНТ 14
#include <stdio.h>
#include <stdlib.h>

long long factorial_iterative(int n) {
    if (n == 0 || n == 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long factorial_recursive(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial_recursive(n - 1);
}

long long combination_iterative(int n, int k) {
    if (k < 0 || k > n) return 0;
    long long numerator = factorial_iterative(n);
    long long denominator = factorial_iterative(k) * factorial_iterative(n - k);
    
    return numerator / denominator;
}

long long combination_recursive(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    // C(n, k) = C(n-1, k-1) + C(n-1, k)
    return combination_recursive(n - 1, k - 1) + combination_recursive(n - 1, k);
}

int main() {

    system("chcp 65001 > nul");

    int n, k;
    int valid = 0;
    
    printf("Вычисление биномиального коэффициента C(n, k)\n");
    
    while (!valid) {
        printf("Введите n и k (через пробел): ");
        int result = scanf("%d %d", &n, &k);
        
        if (result != 2) {
            while (getchar() != '\n');
            printf("Ошибка: введите два целых числа\n");
        }
        else if (n < 0 || k < 0 || k > n) {
            printf("Ошибка: значения должны удовлетворять 0 <= k <= n\n");
        } else {
            valid = 1;
        }
    }
    
    printf("\n--- Результаты вычислений ---\n");
    
    long long result_iter = combination_iterative(n, k);
    printf("Обычный способ: C(%d, %d) = %lld\n", n, k, result_iter);
    
    long long result_rec = combination_recursive(n, k);
    printf("Рекурсивный способ: C(%d, %d) = %lld\n", n, k, result_rec);

  /*printf("%d! (итеративно) = %lld\n", n, factorial_iterative(n));
    printf("%d! (рекурсивно) = %lld\n", n, factorial_recursive(n));
    printf("%d! = %lld\n", k, factorial_iterative(k));
    printf("%d! = %lld\n", n - k, factorial_iterative(n - k)); */ 
    
    return 0;
}