#ifndef INNERHEADER_H
#define INNERHEADER_H
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

extern const char* ErrorNames[];

enum ErrorCodes {
    StackIsEmpty = 1,
    InvalidElementSize
};

typedef struct {
    unsigned char* stack;
    unsigned char* LastBytePtr;
    int ElemSize;
    int ElemCount;
    int MaxElem;
} Stack;

void Construct(Stack*);
void Destruct(Stack*);
void Extend(Stack*);
void Print (const Stack*, void*);
void Copy(Stack*, unsigned char*);
void Paste(Stack*, const unsigned char*);

#endif //INNERHEADER_H