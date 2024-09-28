#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "run.h"
#include <stdint.h>
#define MAX_ADDRESS 0x50000
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


MemEntry *mem_entries;
int pc_counter = 0;

char* deepCopyString(char* str) {
    // Allocate memory for the new string (+1 for the null terminator)
    char* newStr = (char*)malloc(strlen(str) + 1);
    if (newStr == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    
    // Copy the original string into the new memory
    strcpy(newStr, str);
    return newStr;
}
int main() {
    mem_entries = malloc(MAX_ADDRESS * sizeof(MemEntry));
    if (mem_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    char command[MAX_INPUT_SIZE];
    int stepper = 0;
    int *counter_ptr;
    int counter = 0;
    counter_ptr = &counter;
    
    while (1) {  
        fgets(command,sizeof(command),stdin);   
        FILE *input;   
        char **tokens_comm = string_split(command);
        //printf("%s",tokens_comm[0]);
           if(strcmp(tokens_comm[0], "load") == 0){
            pc_counter=0;
            counter=0;
            //printf("%s\n",tokens_comm[0]);
            input = fopen(tokens_comm[1], "r");
            memcpy(register_value,default_register_value,sizeof(register_value));
            //printf("runcheck1\n");
            if(input==NULL){
                printf("error opening file\n");
                fclose(input);
            }
            for(int k=0;k<MAX_ADDRESS;k++){
                mem_entries[k].address=k;
                mem_entries[k].value=0;
            }
            
            continue;
           } else if(strcmp(tokens_comm[0],"run")==0){
                if (input == NULL) {
                    printf("input file not found\n");
                    fclose(input);
                }
                //printf("runcheck2\n");
                char line[MAX_INPUT_SIZE];
                char *array_of_lines[MAX_LINES];
                for (int k = 0; k < MAX_LINES; k++) {
                     array_of_lines[k] = malloc(MAX_INPUT_SIZE * sizeof(char));
                    if (array_of_lines[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for line %d\n", k);
                    return 1; // Exit on allocation failure
                    }
                }
                int i=0;
                uint64_t d_address=0x10000;
                pc_counter=0;
                char *label_names[MAX_LINES];
                int label_position_iter = 0;
                int line_iterater = 0;
                int label_line_numbers[MAX_LINES];
                while (fgets(line, sizeof(line), input) != NULL) {
                    if (ischarinstring(line, ':')) {
                    char *label_selector = strtok(line, ":\n\r");
                    label_names[label_position_iter] = strdup(label_selector);
                    label_line_numbers[label_position_iter] = line_iterater;
                    label_position_iter++;
                    }
                line_iterater++;
                }
                rewind(input);
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
                    //printf("xs ios %d\n", counter);
                    //printf("%s\n",array_of_lines[counter]);
                    int size=strlen(array_of_lines[counter]);
                    //printf("pc at while%d\n",pc_counter);
                    // if(line[0]=='.'){
                    //     char line_copy2[size];
                    //     strcpy(line_copy2,line);
                    // }
                    char *instruction;
                    char *label;
                    
                    char line_copy1[size];
                    strcpy(line_copy1,array_of_lines[counter]);
                    char *copy=deepCopyString(array_of_lines[counter]);
                    char *tokens_for_labels = strtok(copy, ":\n\r");
                    if(ischarinstring(line_copy1,':')==1){
                        label = tokens_for_labels;
                        tokens_for_labels = strtok(NULL, ":\n\r");
                        instruction = tokens_for_labels;
                    } else {
                        instruction = tokens_for_labels;
                        label=NULL;
                    }  
                    //int size2=strlen(instruction);
                    char *instruction_copy;
                    instruction_copy=deepCopyString(instruction);  
                    if(instruction != NULL){
                        for (char *p = instruction; *p; p++) {
                            if (*p == ',') *p = ' ';
                            if (*p == '(') *p = ' ';
                            if (*p == ')') *p = ' ';
                        }    
                    }
                    if (instruction == NULL) continue;
                    
                    char **tokens = string_split(instruction);
                    //printf("%s\n",instruction);
                    //instruction=deepCopyString(instruction_copy);
                    //printf("%s\n",instruction);
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
                                //printf("%lX\n",num);
                                int m = 0;
                                for ( m = 0; m < 8; m++) {
                                    // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //printf("%d\n", m);
                                //j=j+8;
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
                                   // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //printf("%d\n", m);
                                //j = j + 4;
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
                                    // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //j = j + 2;
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
                                    // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //j = j + 1;
                                d_address = d_address + 1;
                            }
                        }
                        counter++;
                    }
                    //printf("runcheck1\n");
                    run_instruction(instruction_copy,tokens, register_value,mem_entries,&pc_counter, label_names, label_line_numbers, counter_ptr,label_position_iter);
                    //printf("runcheck1\n");
                    counter++;
                    //printf("%d\n", counter);
                    //printf("ptr is %d\n", *counter_ptr);
                    
                }
                // for (int j = 0; array_of_lines!=NULL; j++) {
                //         //printf("Line %d: %s\n", j + 1, array_of_lines[j]);
                //         free(array_of_lines[j]); // Free the allocated memory
                //     }
                } else if(strcmp(tokens_comm[0],"regs")==0){
                if (input == NULL) {
                    printf("input file not found\n");
                    fclose(input);
                }
                printf("Registers:\n");
                for(int i = 0; i < 32; i++){
                    printf("x%d = 0x%016lx\n", i, register_value[i]);
                }
                printf("\n");
           } else if(strcmp(tokens_comm[0],"exit")==0){
                printf("RISCV Simulator successfully exited\n");
                return 0;
           } else if(strcmp(tokens_comm[0],"mem")==0){
            char* endpointer;
                uint64_t mem=strtoul(tokens_comm[1],&endpointer,16);
                int index=0;
                int count=atoi(tokens_comm[2]);
                for(int k=0;k<count;k++){
                    printf("Memory[0x%05X] = 0x%02X\n",mem_entries[mem+k].address,mem_entries[mem+k].value);
                }
           } else if (strcmp(tokens_comm[0], "step") == 0){
            stepper = counter;
                //printf("%d\n", stepper);
                if (input == NULL) {
                    printf("input file not found\n");
                    fclose(input);
                }
                //printf("runcheck2\n");
                //printf("check1\n");
                char line[MAX_INPUT_SIZE];
                char *array_of_lines[MAX_LINES];
                for (int k = 0; k < MAX_LINES; k++) {
                     array_of_lines[k] = malloc(MAX_INPUT_SIZE * sizeof(char));
                    if (array_of_lines[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for line %d\n", k);
                    return 1; // Exit on allocation failure
                    }
                }
                //printf("check2\n");
                int i=0;
                uint64_t d_address=0x10000;
                char *label_names[MAX_LINES];
                int label_position_iter = 0;
                int line_iterater = 0;
                int label_line_numbers[MAX_LINES];
                //printf("check3\n");
                while (fgets(line, sizeof(line), input) != NULL) {
                    if (ischarinstring(line, ':')) {
                    char *label_selector = strtok(line, ":\n\r");
                    label_names[label_position_iter] = strdup(label_selector);
                    label_line_numbers[label_position_iter] = line_iterater;
                    label_position_iter++;
                }
                line_iterater++;
                }
                rewind(input);
                //printf("check4\n");
                while(fgets(line,sizeof(line),input)!=NULL){
                    //printf("runcheck\n");
                    char *substrings = strtok(line,"\t\n\r"); 
                    if(substrings!=NULL){
                       strcpy(array_of_lines[i],line);
                       //printf("runcheck1\n");
                       //printf("%s\n",array_of_lines[i]);
                       i++;
                    }
                    if (stepper == 0){
                        if (strcmp(substrings, ".text") == 0) {
                            stepper = i; 
                            continue; 
                        }
                    }
                    if(i>=60){
                        break;
                    }
                }
                int size=strlen(array_of_lines[stepper]);
                char *instruction;
                    char *label;
                    //printf("check5\n");
                    char line_copy1[size];
                    strcpy(line_copy1,array_of_lines[stepper]);
                    char *tokens_for_labels = strtok(array_of_lines[stepper], ":\n\r");
                    
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
                    //printf("%s\n", instruction);
                    if (instruction == NULL) continue;

                    char **tokens = string_split(instruction);
                    
                    if(strcmp(tokens[0],".data")==0 || strcmp(tokens[0],".text")==0){
                        //stepper++;
                        //printf("%d\n", stepper);
                        //counter++;
                        continue;
                    }
                    if(strcmp(tokens[0],".dword")==0){
                         if(tokens[1]==NULL){
                             //error handling?
                         } else {
                            for(int k=1;tokens[k]!=NULL;k++){
                                char* endpointer;
                                long int num = strtol(tokens[k],&endpointer,0);
                                //printf("%lX\n",num);
                                int m = 0;
                                for ( m = 0; m < 8; m++) {
                                    // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //printf("%d\n", m);
                                //j=j+8;
                                d_address=d_address+8;  
                            }
                        }
                         //stepper++;
                         //counter++;
                    } else if(strcmp(tokens[0],".word")==0){
                        if (tokens[1] == NULL){
                            //error handling.
                        } else{
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 4; m++){
                                   // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //printf("%d\n", m);
                                //j = j + 4;
                                d_address = d_address + 4;
                            }
                        }

                        //stepper++;
                        //counter++;
                    }  else if(strcmp(tokens[0],".half")==0){
                        if (tokens[1] == NULL){
                            //error handling.
                        } else{
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 2; m++){
                                    // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //j = j + 2;
                                d_address = d_address + 2;
                            }
                        }

                        //stepper++;
                        //counter++;
                    }  else if(strcmp(tokens[0],".byte")==0){
                        if (tokens[1] == NULL){
                            //error handling.
                        } else{
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 1; m++){
                                    // mem_entries[j+m].address=d_address+m;
                                    // mem_entries[j+m].value = (num >> (m * 8)) & 0xFF; 
                                    //mem_entries[j+m].address=d_address+m;
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                    //printf("0x%llX->0x%02X\n",mem_entries[d_address+m].address,mem_entries[d_address+m].value);
                                }
                                //j = j + 1;
                                d_address = d_address + 1;
                            }
                        }
                        //stepper++;
                        //counter++;
                    }
void run_instruction(char* line,char **tokens, long int register_value[],MemEntry  *mem_entries,int *pc_counter, char **label_names, int label_line_numbers[], int *counter_ptr,int label_position_iter);
                    run_instruction(line_copy1,tokens, register_value,mem_entries,&pc_counter, label_names, label_line_numbers, counter_ptr,label_position_iter);
                    stepper++;
                    counter = stepper;
                    printf("%d\n", stepper);
                    printf("%d\n", counter);      
           }
    }
    free(mem_entries);
    return 0;
}
