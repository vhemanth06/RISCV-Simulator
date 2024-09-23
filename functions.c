#include<stdio.h>
#include <string.h>
#define MAX_TOKENS 10
char **string_split(char *string) {
    static char *result[MAX_TOKENS + 1];
    int a = 0;
    char *substrings = strtok(string," \t\n\r"); 
    
    while (substrings != NULL && a < MAX_TOKENS) {
        result[a++] = substrings;
        substrings = strtok(NULL," \t\n\r");
    }
    result[a] = NULL;
    return result;
}
