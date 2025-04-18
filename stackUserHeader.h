#ifndef STACKUSERHEADER_H
#define STACKUSERHEADER_H

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

typedef struct Stack Stack;

void Push(Stack*, const void*);
void* Pop(Stack*, void*);
int Size(const Stack*);
void Construct(Stack*, int);
void Destruct(Stack*);
void PrintStack(Stack*);
#endif