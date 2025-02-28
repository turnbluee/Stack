#include <string.h>
#include "UserHeader.h"
#include "FuncsHeader.h"

const char* ErrorNames[] = {
    "Zero element",
    "Stack is empty",
    "Invalid element size",
    "Stack allocation error",
    "Stack extending error",
    "LastElem allocation error",
};

const char* get_filename(const char* path) {
    const char* filename = strrchr(path, '/');
    return filename + 1;
}

void Construct (Stack* stack) {
    printf("%s", "Stack element size:");
    scanf("%i", &stack->ElemSize);
    stack->MaxElem = INITIAL_ELEM_COUNT;
    stack->ElemCount = 0;
    stack->stack = malloc(stack->ElemSize * stack->MaxElem);
    if (stack->stack == NULL) {
        ERROR_PROCESSING(StackAllocationError);
    }
}

void Push (Stack* stack, const void* value) {
    if (stack->ElemCount == stack->MaxElem - 1) {
        Extend(stack);
    }

    Copy(stack->stack + stack->ElemCount * stack->ElemSize, value, stack->ElemSize);
    ++stack->ElemCount;
}

void Extend (Stack* stack) {
    stack->MaxElem *= 2;
    stack->stack = (unsigned char*)realloc(stack->stack, stack->MaxElem * stack->ElemSize);
    if (stack->stack == NULL) {
        ERROR_PROCESSING(StackExtendingError);
    }
}

void* Pop (Stack* stack) {
    if (stack->ElemCount < 1) {
        ERROR_PROCESSING(StackIsEmpty);
    }

    void* CopyingElem = malloc(stack->ElemSize);
    if(CopyingElem == NULL) {
        ERROR_PROCESSING(LastElemAllocationError);
    }

    Copy(CopyingElem, stack->stack + (stack->ElemCount - 1) * stack->ElemSize, stack->ElemSize);
    --stack->ElemCount;

    Print(stack->ElemSize, CopyingElem);
    return CopyingElem;
}

void Copy(unsigned char* CopyTo, const unsigned char* CopyFrom, int ElemSize) {
    for(int i = 0; i < ElemSize; i++) {
        CopyTo[i] = CopyFrom[i];
    }
}

void Print (const int ElemSize, void* LastElem) {
    switch (ElemSize) {
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

void PrintStack (Stack* stack) {
    for (int i = 1; i <= stack->ElemCount; i++) {
        Print(stack->ElemSize, stack->stack + (stack->ElemCount - i) * stack->ElemSize);
    }
}