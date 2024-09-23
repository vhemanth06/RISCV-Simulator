#include <stdio.h>
#include <string.h>
#include "functions.h"

#define MAX_TOKENS 10
#define MAX_INPUT_SIZE 100 

int main() {
    char command[MAX_INPUT_SIZE]; 
    while (1) {  
        fgets(command,sizeof(command),stdin);   
        FILE *input;   
        char **tokens = string_split(command);
           if(strcmp(tokens[0], "load") == 0){
            //printf("%s\n",tokens[0]);
            input = fopen(tokens[1], "r");
           } else if(strcmp(tokens[0],"run")){
                if (input == NULL) {
                    printf("input file not found\n");
                }
                char line[MAX_INPUT_SIZE];
                int instr_counter=1;
                while(fgets(line,sizeof(line),input)!=NULL){
                    
                }
                
           } else if(strcmp(tokens[0],"reg")){
                if (input == NULL) {
                    printf("input file not found\n");
                }
           } else if(strcmp(tokens[0],"exit")){
                printf("RISCV Simulator successfully exited\n");
                break;
           }
    }
    return 0;
}
