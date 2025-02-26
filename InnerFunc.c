#include <string.h>
#include "MainHeader.h"
#include "InnerHeader.h"

const char* ErrorNames[] = {
    "Zero element",
    "Stack is empty",
    "Invalid element size"
};

const char* get_filename(const char* path) {
    const char* filename = strrchr(path, '/');
    return filename + 1;
}

// TODO: копирование

void Construct (Stack* stack) {
    int size;
    printf("%s", "Stack element size:");
    scanf("%i", &size);
    stack->ElemSize = size;
    stack->MaxElem = INITIAL_ELEM_COUNT;
    stack->ElemCount = 0;
    stack->stack = malloc(stack->ElemSize * stack->MaxElem);
    assert(stack->stack != NULL && "Stack allocation error");
    stack->LastBytePtr = stack->stack;
}

void Push (Stack* stack, void* value) {
    if (stack->ElemCount == stack->MaxElem - 1) {
        Extend(stack);
    }

    unsigned char* ValuePtr = (unsigned char*)value;

    for (int i = 0; i < stack->ElemSize; ++i) {
        ++stack->LastBytePtr;
        *stack->LastBytePtr = *ValuePtr;
        ++ValuePtr;
    }
    ++stack->ElemCount;
}

void Extend (Stack* stack) {
    stack->MaxElem *= 2;
    stack->stack = (unsigned char*)realloc(stack->stack, stack->MaxElem * stack->ElemSize);
    assert(stack->stack != NULL && "Stack extending error");
}

void* Pop (Stack* stack) {
    if (stack->ElemCount < 1) {
        ERROR_PROCESSING(StackIsEmpty);
    }

    unsigned char* LastElem = malloc(stack->ElemSize);
    assert(LastElem != NULL && "LastElem allocation error");

    for(int i = stack->ElemSize - 1; i >= 0; --i) {
        *(LastElem + i) = *stack->LastBytePtr;
        --stack->LastBytePtr;
    }
    --stack->ElemCount;

    assert(LastElem != NULL && "LastElem getting error");
    Print(stack, LastElem);
    return LastElem;
}

void Print (const Stack* stack, void* LastElem) {
    switch (stack->ElemSize) {
        case(sizeof(int)): {
            int *Elem = (int*)LastElem;
            printf("%i\n", *Elem);
            break;
        }
        case(sizeof(char)): {
            char *Elem = (char*)LastElem;
            printf("%c\n", *Elem);
            break;
        }
        case(sizeof(double)): {
            double *Elem = (double*)LastElem;
            printf("%f\n", *Elem);
            break;
        }
        default: {
            ERROR_PROCESSING(InvalidElementSize);
        }
    }
}

void Destruct (Stack* stack) {
    free(stack->stack);
    free(stack);
}

int Size (const Stack* stack) {
    return stack->ElemCount;
}