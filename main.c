#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "run.h"
#include <stdint.h>

#define MAX_TOKENS 10
#define MAX_INPUT_SIZE 100 
long int default_register_value[]={0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,};
long int register_value[]={0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,};
uint64_t mem_address[50000];
long int mem_values[50000];

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
            memcpy(register_value,default_register_value,sizeof(register_value));
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
                char *array_of_lines[60];

                uint64_t d_address=0x10000;
                while(fgets(line,sizeof(line),input)!=NULL){
                     
                }
                while(fgets(line,sizeof(line),input)!=NULL){
                    if(strcmp(line,".data")==0){
                        continue;
                    }
                    if(strcmp(line,".text")==0){
                        break;
                    }
                    if(line != NULL){
                        for (char *p = line; *p; p++) {
                            if (*p == ',') *p = ' ';
                        }    
                    }

                    if (line == NULL) continue;
                    char **data=string_split(line);
                    if(strcmp(data,".dword")==0){

                    } else if(strcmp(data,".word")==0){

                    } else if(strcmp(data,".half")==0){

                    } else if(strcmp(data,".byte")==0){

                    } 
                }
                while(fgets(line,sizeof(line),input)!=NULL){
                    int size=strlen(line);
                    if(strcmp(line,".data")==0 || strcmp(line,".text")==0){
                        continue;
                    }
                    // if(line[0]=='.'){
                    //     char line_copy2[size];
                    //     strcpy(line_copy2,line);
                    // }
                    char *instruction;
                    char *label;
                    
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
                    if(strcmp(tokens[0],".dword")==0 ||strcmp(tokens[0],".word")==0 ||strcmp(tokens[0],".half")==0 ||strcmp(tokens[0],".byte")==0){
                         continue;
                    }
                    run_instruction(tokens, register_value,mem_address,mem_values);
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
