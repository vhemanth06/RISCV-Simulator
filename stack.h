#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define MAX_STACK_SIZE 50

typedef struct Stack
{
    char *value[MAX_STACK_SIZE];
    int top_index;
    int line_num[MAX_STACK_SIZE];
} Stack;

Stack *createEmptyStack();
bool push(Stack *S, char *string);
char *pop(Stack *S);
char *top(Stack *S);

#endif
