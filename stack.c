
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

Stack* createEmptyStack() {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL; 
    }
    stack->top_index = -1; 
    return stack;
}

bool push(Stack *S, char* string) {
    if (S->top_index >= (MAX_STACK_SIZE - 1)) {
        return false; 
    }
    S->value[++(S->top_index)] = string;
    return true;
}

char* pop(Stack *S) {
    if (S->top_index == -1) {
        printf("Empty Call Stack: Execution completed\n");
        return NULL; 
    }
    return S->value[(S->top_index)--]; 
}

char* top(Stack *S) {
    if (S->top_index == -1) {
        return NULL; 
    }
    return S->value[S->top_index]; 
}
