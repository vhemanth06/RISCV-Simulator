// #include<stdio.h>
// #include<stdlib.h>
// #include<stdbool.h>
// #include <math.h>


// #define MAX_STACK_SIZE 500

// typedef struct Stack
// {
//     char* value[MAX_STACK_SIZE];
//     int top_index;
//     int line_num;
// }Stack;

// Stack* createEmptyStack();
// bool push(Stack *S,char* string);
// char* pop(Stack *S);
// char* top(Stack *S);
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define MAX_STACK_SIZE 50

typedef struct Stack {
    char* value[MAX_STACK_SIZE];
    int top_index;
    int line_num[MAX_STACK_SIZE];  // Consider if you need this
} Stack;

// Function prototypes
Stack* createEmptyStack();
bool push(Stack *S, char* string);
char* pop(Stack *S);
char* top(Stack *S);

#endif // STACK_H
