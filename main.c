#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "run.h"

#define MAX_TOKENS 10
#define MAX_INPUT_SIZE 100 
long int register_value[]={0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,};

int main() {
    char command[MAX_INPUT_SIZE]; 
    while (1) {  
        fgets(command,sizeof(command),stdin);   
        FILE *input;   
        char **tokens_comm = string_split(command);
        //printf("%s",tokens_comm[0]);
           if(strcmp(tokens_comm[0], "load") == 0){
            //printf("%s\n",tokens_comm[0]);
            input = fopen(tokens_comm[1], "r");
            //printf("runcheck1\n");
            if(input==NULL){
                printf("error opening file\n");
                fclose(input);
            }
            continue;
           } else if(strcmp(tokens_comm[0],"run")==0){
                if (input == NULL) {
                    printf("input file not found\n");
                    fclose(input);
                }
                //printf("runcheck2\n");
                char line[MAX_INPUT_SIZE];
                int instr_counter=1;
                while(fgets(line,sizeof(line),input)!=NULL){
                    char *instruction;
                    char *label;
                    int size=strlen(line);
                    char line_copy1[size];
                    strcpy(line_copy1,line);
                    char *tokens_for_labels = strtok(line, ":\n\r");
                    if(ischarinstring(line_copy1,':')==1){
                        label = tokens_for_labels;
                        tokens_for_labels = strtok(NULL, ":\n\r");
                        instruction = tokens_for_labels;
                    } else {
                        instruction = tokens_for_labels;
                        label=NULL;
                    }    
                    if(instruction != NULL){
                        for (char *p = instruction; *p; p++) {
                            if (*p == ',') *p = ' ';
                            if (*p == '(') *p = ' ';
                            if (*p == ')') *p = ' ';
                        }    
                    }
                    if (instruction == NULL) continue;
                    char **tokens = string_split(instruction);
                    run_instruction(tokens, register_value);
                }
                
           } else if(strcmp(tokens_comm[0],"reg")==0){
                if (input == NULL) {
                    printf("input file not found\n");
                }
           } else if(strcmp(tokens_comm[0],"exit")==0){
                printf("RISCV Simulator successfully exited\n");
                return 0;
           }
    }
    return 0;
}
