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
    "Hash corrupted, some field(s) corrupted",
    "Magic corrupted, possibly use-after-free",
    "Canary corrupted, possibly sigmentation fault",
    "Read a poisoned slot",
    "Uninitialized slot between initialized ones",
    "Bitmask allocation error"
};

const char* get_filename(const char* path) {
    const char* filename = strrchr(path, '/');
    return filename + 1;
}

void Construct (Stack* stack, int elemSize) {
    stack->leftCanary = CANARY;
    stack->rightCanary = CANARY;

    uint64_t magicFirstHalf = ((uint64_t)rand() << 32);
    uint64_t magicSecondHalf =  (uint64_t)rand();
    stack->magic = magicFirstHalf | magicSecondHalf;
    stack->invMagic = ~stack->magic;

    stack->elemSize = elemSize;
    stack->maxElem = INITIAL_ELEM_COUNT;
    stack->elemCount = 0;
    stack->stack = malloc(stack->elemSize * stack->maxElem);

    size_t mask_bytes = (stack->maxElem + 7) / 8;
    stack->bitmask = calloc(mask_bytes, sizeof *stack->bitmask);
    if (stack->bitmask == NULL) {
        ERROR_PROCESSING(BitmaskAllocationError);
    }

    if (stack->stack == NULL) {
        ERROR_PROCESSING(StackAllocationError);
    }
    hashKey(&stack->hash, (const unsigned char*)stack,sizeof (*stack));

}

void checkStack (Stack* stack) {
    if (stack->magic != ~stack->invMagic) {
        ERROR_PROCESSING(MagicCorrupted);
    }

    if (stack->rightCanary != CANARY ||
        stack->leftCanary != CANARY) {
        ERROR_PROCESSING(CanaryCorrupted);
    }

    uint32_t currHash = stack->hash;
    hashKey(&stack->hash, (const unsigned char*)stack, sizeof (*stack));
    if (currHash != stack->hash) {
        ERROR_PROCESSING(HashCorrupted);
    }

    for (size_t i = stack->elemCount; i < stack->maxElem; i++) {
        if (bitmaskTest(stack->bitmask, i)) {
            ERROR_PROCESSING(BitmaskHoleDetected);
        }
    }

}

void Push (Stack* stack, const void* value) {
    checkStack(stack);

    if (stack->elemCount == stack->maxElem - 1) {
        Extend(stack);
    }

    Copy(stack->stack + stack->elemCount * stack->elemSize, value, stack->elemSize);
    bitmaskPush(stack->bitmask, stack->elemCount);
    ++stack->elemCount;
    hashKey(&stack->hash, (const unsigned char*)stack,sizeof (*stack));
}

void Extend (Stack* stack) {
    size_t oldMax = stack->maxElem;
    stack->maxElem *= 2;
    unsigned char* newBuf = realloc(stack->stack,stack->maxElem * stack->elemSize);
    if (!newBuf) {
        ERROR_PROCESSING(StackExtendingError);
    }
    stack->stack = newBuf;

    size_t oldMaskBytes = (oldMax + 7) / 8;
    size_t newMaskBytes = (stack->maxElem + 7) / 8;
    uint8_t* newMask = realloc(stack->bitmask, newMaskBytes);
    if (!newMask) {
        ERROR_PROCESSING(StackExtendingError);
    }
    stack->bitmask = newMask;
    memset(newMask + oldMaskBytes, 0, newMaskBytes - oldMaskBytes);
}

void* Pop (Stack* stack, void* poppedElem) {
    checkStack(stack);

    if (stack->elemCount < 1) {
        ERROR_PROCESSING(StackIsEmpty);
    }

    if (!bitmaskTest(stack->bitmask, stack->elemCount - 1)) {
        ERROR_PROCESSING(ReadingPoisonedSlot);
    }

    Copy(poppedElem, stack->stack + (stack->elemCount - 1) * stack->elemSize, stack->elemSize);
    bitmaskPop(stack->bitmask, stack->elemCount - 1);
    --stack->elemCount;
    hashKey(&stack->hash, (const unsigned char*)stack, sizeof(*stack));

    Print(stack->elemSize, poppedElem);

    return poppedElem;
}

void Copy(unsigned char* copyTo, const unsigned char* copyFrom, size_t elemSize) {
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
    free(stack->bitmask);
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

void hashKey (uint32_t* hash, const unsigned char* stackPtr, const size_t dataLen) {
    *hash = 2166136261u;
    const uint32_t fnvPrime = 16777619u;
    size_t offset = (const unsigned char*)hash - stackPtr;
    size_t skipEnd = offset + sizeof (*hash);
    size_t leftCanarySize  = sizeof(uint64_t);
    size_t rightCanarySize = sizeof(uint64_t);

    for (size_t i = leftCanarySize; i < offset; i++) {
        *hash ^= *(stackPtr + i);
        *hash *= fnvPrime;
    }
    for (size_t i = skipEnd; i < dataLen - rightCanarySize; i++) {
        *hash ^= *(stackPtr + i);
        *hash *= fnvPrime;
    }
}

void bitmaskPush (uint8_t* bitmask, size_t elemAddr) {
    bitmask[elemAddr / 8] |= 1u << elemAddr % 8;
}

void bitmaskPop (uint8_t* bitmask, size_t elemAddr) {
    bitmask[elemAddr / 8] &= ~(1u << elemAddr % 8);
}

int bitmaskTest (const uint8_t* bitmask, size_t elemAddr) {
    return (bitmask[elemAddr / 8] >> (elemAddr % 8)) & 1;
}