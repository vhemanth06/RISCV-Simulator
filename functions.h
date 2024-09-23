#include<stdio.h>
#include<ctype.h>
#define MAX_TOKENS 10
char **string_split(char *string);
int non_int_char_finder(char *str);
int ischarinstring(char *string,char x);
int register_finder(char *str);