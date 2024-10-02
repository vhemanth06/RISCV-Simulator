#include<stdio.h>
#include <string.h>
#include<ctype.h>
#include<stdlib.h>
#define MAX_TOKENS 50
char **string_split(char *string);
int non_int_char_finder(char *str);
int ischarinstring(char *string,char x);
int register_finder(char *str);
char* deepCopyString(char* str);
char* trim_space(char* string);