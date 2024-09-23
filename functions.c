#include<stdio.h>
#include <string.h>
#include<ctype.h>
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
int non_int_char_finder(char *str){
    int len=strlen(str);
    for(int i=0;i<len;i++){
        if(!isdigit(str[i]) && str[i]!='-'){
            return 1;
        }
    }
    return 0;
}
int ischarinstring(char *string,char x){
    for(int i=0;i<strlen(string);i++){
        if(string[i]==x){
            return 1;
        }
    }
    return 0;
}
