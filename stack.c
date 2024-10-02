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

// /* Functions for Stack operations */

// Stack *createEmptyStack(){
//     Stack *stack = (Stack*)malloc(sizeof(Stack));
//     if (stack == NULL) {
//         return NULL;
//     }
//     stack->top_index=-1;
//     return stack;
// }
// bool push(Stack *S,char* string){
//     if(S->top_index>=(MAX_STACK_SIZE-1)){
//         return false;   //error check when pushing into a full stack
//      } else {
//         S->top_index=S->top_index+1;
//         S->value[S->top_index]=string;
//         return true;
//      }  
// }
// char* pop(Stack *S){
//     if (S->top_index==-1){
//         printf("Empty Call Stack: Execution completed\n");
//     }
//     S->top_index=S->top_index-1;
//     char* a = S->value[S->top_index+1];
//     return a;
// }
// char* top(Stack *S){
//     return S->value[S->top_index];
// }
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

Stack* createEmptyStack() {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL; // Handle allocation failure
    }
    stack->top_index = -1; // Initialize top index
    return stack;
}

bool push(Stack *S, char* string) {
    if (S->top_index >= (MAX_STACK_SIZE - 1)) {
        return false; // Stack overflow
    }
    S->value[++(S->top_index)] = string; // Increment top index and assign value
    return true;
}

char* pop(Stack *S) {
    if (S->top_index == -1) {
        printf("Empty Call Stack: Execution completed\n");
        return NULL; // Handle empty stack
    }
    return S->value[(S->top_index)--]; // Return top value and decrement index
}

char* top(Stack *S) {
    if (S->top_index == -1) {
        return NULL; // Handle empty stack
    }
    return S->value[S->top_index]; // Return top value without modifying stack
}
