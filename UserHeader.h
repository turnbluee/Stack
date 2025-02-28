#ifndef MAINHEADER_H
#define MAINHEADER_H

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

typedef struct Stack Stack;

void Push(Stack*, const void*);
void* Pop(Stack*);
int Size(const Stack*);
void Construct(Stack*);
void Destruct(Stack*);
void PrintStack(Stack*);
#endif //MAINHEADER_H