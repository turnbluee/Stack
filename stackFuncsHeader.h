#ifndef STACKFUNCSHEADER_H
#define STACKFUNCSHEADER_H
#define INITIAL_ELEM_COUNT 100

#define ERROR_PROCESSING(error_code) \
do { \
if ((error_code) > 0) { \
fprintf(stderr, "Error: %s (%d).\nFile: %s, line: %d.\n", \
ErrorNames[error_code], error_code, get_filename(__FILE__), __LINE__); \
exit(error_code); \
} \
} while (0)

#include <locale.h>
#include <stdlib.h>
#include <assert.h>
#include "stackUserHeader.h"

extern const char* ErrorNames[];

enum ErrorCodes {
    StackIsEmpty = 1,
    InvalidElementSize,
    StackAllocationError,
    StackExtendingError,
    LastElemAllocationError,

};

struct Stack {
    unsigned char* stack;
    int elemSize;
    int elemCount;
    int maxElem;
};

void Extend(Stack*);
void Print (int, void*);
void Copy(unsigned char*, const unsigned char*, int);

#endif