#include "stackFuncsHeader.h"
#include "stackUserHeader.h"

int main() {
    setlocale(LC_ALL, ".1251");
    Stack* stack = malloc(sizeof(stack));
    int elem1 = 1,
    elem2 = 2,
    elem3 = 3;
    int elemSize = sizeof(elem1);
    Construct(stack, elemSize);
    Push(stack, &elem1);
    Push(stack, &elem2);
    Push(stack, &elem3);
    /*PrintStack (stack);*/
    void* poppedElem = malloc(stack->elemSize);
    Pop(stack, poppedElem);
    Pop(stack, poppedElem);
    Pop(stack, poppedElem);
    Destruct(stack);
    return 0;
}