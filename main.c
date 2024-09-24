#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "run.h"
#include <stdint.h>

#define MAX_TOKENS 10
#define MAX_INPUT_SIZE 100 
#define MAX_LINES 60 
long int default_register_value[]={0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,};
long int register_value[]={0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,};
uint64_t mem_address[50000];
int64_t mem_values[50000];

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
                int counter=0;
                char *array_of_lines[MAX_LINES];
                for (int k = 0; k < MAX_LINES; k++) {
                     array_of_lines[k] = malloc(MAX_INPUT_SIZE * sizeof(char));
                    if (array_of_lines[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for line %d\n", k);
                    return 1; // Exit on allocation failure
                    }
                }
                int i=0,j=0;
                uint64_t d_address=0x10000;
                while(fgets(line,sizeof(line),input)!=NULL){
                    //printf("runcheck\n");
                    char *substrings = strtok(line,"\t\n\r"); 
                    if(substrings!=NULL){
                       strcpy(array_of_lines[i],line);
                    //printf("runcheck1\n");
                    //printf("%s\n",array_of_lines[i]);
                    i++;
                    }
                     
                    if(i>=60){
                        break;
                    }
                }
                // while(array_of_lines[counter]!=0){
                      
                // }
                // while(array_of_lines[counter]!=0){
                //     if(strcmp(line,".data")==0){
                //         continue;
                //     }
                //     if(strcmp(line,".text")==0){
                //         break;
                //     }
                //     if(line != NULL){
                //         for (char *p = line; *p; p++) {
                //             if (*p == ',') *p = ' ';
                //         }    
                //     }

                //     if (line == NULL) continue;
                //     char **data=string_split(line);
                //     if(strcmp(data[0],".dword")==0){

                //     } else if(strcmp(data[0],".word")==0){

                //     } else if(strcmp(data[0],".half")==0){

                //     } else if(strcmp(data[0],".byte")==0){

                //     } 
                // }
                // rewind(input);
                while(counter < i){
                    //printf("%s\n",array_of_lines[counter]);
                    int size=strlen(array_of_lines[counter]);
                    
                    // if(line[0]=='.'){
                    //     char line_copy2[size];
                    //     strcpy(line_copy2,line);
                    // }
                    char *instruction;
                    char *label;
                    
                    char line_copy1[size];
                    strcpy(line_copy1,array_of_lines[counter]);
                    char *tokens_for_labels = strtok(array_of_lines[counter], ":\n\r");
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
                    if(strcmp(tokens[0],".data")==0 || strcmp(tokens[0],".text")==0){
                        counter++;
                        continue;
                    }
                    if(strcmp(tokens[0],".dword")==0){
                         if(tokens[1]==NULL){
                             //error handling?
                         } else {
                            for(int k=1;tokens[k]!=NULL;k++){
                                char* endpointer;
                                long int num = strtol(tokens[k],&endpointer,0);
                                printf("%lX\n",num);
                                int m = 0;
                                for ( m = 0; m < 8; m++) {
                                    mem_address[j+m]=d_address+m;
                                    mem_values[j+m] = (num >> (m * 8)) & 0xFF; 
                                    printf("0x%llX->0x%X\n",mem_address[j+m],mem_values[j+m]);
                                }
                                printf("%d\n", m);
                                j=j+8;
                                d_address=d_address+8;  
                            }
                        }
                         counter++;
                    } else if(strcmp(tokens[0],".word")==0){
                        if (tokens[1] == NULL){
                            //error handling.
                        } else{
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 4; m++){
                                    mem_address[j + m] = d_address + m;
                                    mem_values[j + m] = (num >> (m * 8)) & 0xFF;
                                    printf("0x%llX->0x%X\n",mem_address[j+m],mem_values[j+m]);
                                }
                                //printf("%d\n", m);
                                j = j + 4;
                                d_address = d_address + 4;
                            }
                        }

                        counter++;
                    }  else if(strcmp(tokens[0],".half")==0){
                        if (tokens[1] == NULL){
                            //error handling.
                        } else{
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 2; m++){
                                    mem_address[j + m] = d_address + m;
                                    mem_values[j + m] = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%X\n",mem_address[j+m],mem_values[j+m]);
                                }
                                j = j + 2;
                                d_address = d_address + 2;
                            }
                        }

                        counter++;
                    }  else if(strcmp(tokens[0],".byte")==0){
                        if (tokens[1] == NULL){
                            //error handling.
                        } else{
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 1; m++){
                                    mem_address[j + m] = d_address + m;
                                    mem_values[j + m] = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%X\n",mem_address[j+m],mem_values[j+m]);
                                }
                                j = j + 1;
                                d_address = d_address + 1;
                            }
                        }
                        counter++;
                    }
                    run_instruction(tokens, register_value,mem_address,mem_values);
                    counter++;
                    
                }
                for (int j = 0; j < MAX_LINES; j++) {
                        //printf("Line %d: %s\n", j + 1, array_of_lines[j]);
                        free(array_of_lines[j]); // Free the allocated memory
                    }
                
           } else if(strcmp(tokens_comm[0],"reg")==0){
                if (input == NULL) {
                    printf("input file not found\n");
                    fclose(input);
                }
                printf("Registers:\n");
                for(int i = 0; i < 32; i++){
                    printf("x%d = %ld\n", i, register_value[i]);
                }
                printf("\n");
           } else if(strcmp(tokens_comm[0],"exit")==0){
                printf("RISCV Simulator successfully exited\n");
                return 0;
           }
    }
    return 0;
}
