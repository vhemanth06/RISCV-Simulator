#include <stdio.h>
#include <string.h>
#include "functions.h"

#define MAX_TOKENS 10
#define MAX_INPUT_SIZE 100 

int main() {
    char input[MAX_INPUT_SIZE]; 
    while (1) {  
        fgets(input,sizeof(input),stdin);      
        char **tokens = string_split(input);
           if(strcmp(tokens[0], "load") == 0){
            //printf("%s\n",tokens[0]);
           } else if(strcmp(tokens[0],"run")){

           } else if(strcmp(tokens[0],"reg")){

           } else if(strcmp(tokens[0],"exit")){

           }
    }
    return 0;
}
