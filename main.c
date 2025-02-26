#include "MainHeader.h"



int main() {
    setlocale(LC_ALL, ".1251");
    Stack* stack = malloc(sizeof(Stack));
    Construct(stack);
    int Elem1 = 1,
    Elem2 = 2,
    Elem3 = 3;
    Push(stack, &Elem1);
    Push(stack, &Elem2);
    Push(stack, &Elem3);
    Pop(stack);
    Pop(stack);
    Pop(stack);
    Destruct(stack);
    return 0;
}
