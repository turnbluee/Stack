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
#define CANARY 0xDEADBEEFCAFEBABEULL


#include <locale.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "stackUserHeader.h"

extern const char* ErrorNames[];

enum ErrorCodes {
    StackIsEmpty = 1,
    InvalidElementSize,
    StackAllocationError,
    StackExtendingError,
    LastElemAllocationError,
    HashCorrupted,
    MagicCorrupted,
    CanaryCorrupted,
    ReadingPoisonedSlot,
    BitmaskHoleDetected,
    BitmaskAllocationError
};

struct Stack {
    uint64_t leftCanary;

    uint64_t magic;
    uint64_t invMagic;

    unsigned char* stack;
    size_t elemSize;
    size_t elemCount;
    size_t maxElem;

    uint32_t hash;
    uint8_t* bitmask;

    uint64_t rightCanary;
};

void Extend(Stack*);
void Print (int, void*);
void Copy(unsigned char*, const unsigned char*, size_t);
void hashKey (uint32_t*, const unsigned char*, size_t dataLen);
void bitmaskPush(uint8_t*, size_t);
void bitmaskPop(uint8_t*, size_t);
int bitmaskTest(const uint8_t*, size_t);

#endif