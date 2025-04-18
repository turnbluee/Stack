#include <string.h>
#include "stackUserHeader.h"
#include "stackFuncsHeader.h"

const char* ErrorNames[] = {
    "Zero element",
    "Stack is empty",
    "Invalid element size",
    "Stack allocation error",
    "Stack extending error",
    "lastElem allocation error",
};

const char* get_filename(const char* path) {
    const char* filename = strrchr(path, '/');
    return filename + 1;
}

void Construct (Stack* stack, int elemSize) {
    stack->elemSize = elemSize;
    stack->maxElem = INITIAL_ELEM_COUNT;
    stack->elemCount = 0;
    stack->stack = malloc(stack->elemSize * stack->maxElem);
    if (stack->stack == NULL) {
        ERROR_PROCESSING(StackAllocationError);
    }
}

void Push (Stack* stack, const void* value) {
    if (stack->elemCount == stack->maxElem - 1) {
        Extend(stack);
    }

    Copy(stack->stack + stack->elemCount * stack->elemSize, value, stack->elemSize);
    ++stack->elemCount;
}

void Extend (Stack* stack) {
    stack->maxElem *= 2;
    stack->stack = (unsigned char*)realloc(stack->stack, stack->maxElem * stack->elemSize);
    if (stack->stack == NULL) {
        ERROR_PROCESSING(StackExtendingError);
    }
}

void* Pop (Stack* stack, void* poppedElem) {
    if (stack->elemCount < 1) {
        ERROR_PROCESSING(StackIsEmpty);
    }

    Copy(poppedElem, stack->stack + (stack->elemCount - 1) * stack->elemSize, stack->elemSize);
    --stack->elemCount;

    Print(stack->elemSize, poppedElem);

    return poppedElem;
}

void Copy(unsigned char* copyTo, const unsigned char* copyFrom, int elemSize) {
    for(int i = 0; i < elemSize; i++) {
        copyTo[i] = copyFrom[i];
    }
}

void Print (const int elemSize, void* lastElem) {
    switch (elemSize) {
        case(sizeof(int)): {
            int *elem = (int*)lastElem;
            printf("%i\n", *elem);
            break;
        }
        case(sizeof(char)): {
            char *elem = (char*)lastElem;
            printf("%c\n", *elem);
            break;
        }
        case(sizeof(double)): {
            double *elem = (double*)lastElem;
            printf("%f\n", *elem);
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
    return stack->elemCount;
}

void PrintStack (Stack* stack) {
    for (int i = 1; i <= stack->elemCount; i++) {
        Print(stack->elemSize, stack->stack + (stack->elemCount - i) * stack->elemSize);
    }
}