#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_TOKENS 50
char **string_split(char *string)
{
    static char *result[MAX_TOKENS + 1];
    int a = 0;
    char *substrings = strtok(string, " \t\n\r");

    while (substrings != NULL && a < MAX_TOKENS)
    {
        result[a++] = substrings;
        substrings = strtok(NULL, " \t\n\r");
    }
    result[a] = NULL;
    return result;
}
int non_int_char_finder(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(str[i]) && str[i] != '-')
        {
            return 1;
        }
    }
    return 0;
}
int ischarinstring(char *string, char x)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == x)
        {
            return 1;
        }
    }
    return 0;
}
int register_finder(char *str)
{
    if (str == NULL || *str == '\0')
    {
        return -1;
    }
    const char *registers_format1[] = {"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
                                       "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15",
                                       "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23",
                                       "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31"};
    const char *registers_format2[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
                                       "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
                                       "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
                                       "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
    for (int i = 0; i <= 31; i++)
    {
        if (strcmp(str, registers_format1[i]) == 0 || strcmp(str, registers_format2[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

char *deepCopyString(char *str)
{
    char *newStr = (char *)malloc(strlen(str) + 1);
    if (newStr == NULL)
    {
        return NULL;
    }

    strcpy(newStr, str);
    return newStr;
}

char *trim_space(char *string)
{
    if (string == NULL)
    {
        printf("null\n");
        return NULL;
    }
    while (isspace((unsigned char)*string))
    {
        string++;
    }
    return string;
}