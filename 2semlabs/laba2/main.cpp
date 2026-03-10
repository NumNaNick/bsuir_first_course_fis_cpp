#include <iostream>
#include <stdio.h>

int *stack;
int tos = 0;
int MAX;

void push(int i)
{
  if(tos >= MAX) {
    printf("Стек полон\n");
    return;
  }
  stack[tos] = i;
  tos++;
}
/* Получить верхний элемент стека. */
int pop(void)
{
  if(tos <= 0) {
    printf("Стек пуст\n");
    return 0;
  }
  tos--;
  return stack[tos];
}

int main() {

    std::cout << "--- Введите размер стека: ";
    std::cin >> MAX;

    stack = new int[MAX];

    push(10);
    push(20);
    push(30);

    printf("Popped: %d\n", pop());
    printf("Popped: %d\n", pop());
    printf("Popped: %d\n", pop());
    printf("Popped: %d\n", pop());

    delete[] stack;
    return 0;
}