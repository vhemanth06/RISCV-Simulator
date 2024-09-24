#include <stdio.h>
#include <string.h>
#include "functions.h"

#define MAX_TOKENS 10
#define MAX_INPUT_SIZE 100 
int register_value[]={0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,};

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
                    if (strcmp(tokens[0], "add") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);

                        register_value[rd] == register_value[rs1] +  register_value[rs2];
            
                    } else if (strcmp(tokens[0], "sub") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);

                        register_value[rd] == register_value[rs1] -  register_value[rs2];
           
                    } else if (strcmp(tokens[0], "and") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);

                        register_value[rd] == register_value[rs1] &  register_value[rs2];
            
                    } else if (strcmp(tokens[0], "or") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);

                        register_value[rd] == register_value[rs1] |  register_value[rs2];
           
                    } else if (strcmp(tokens[0], "xor") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);

                        register_value[rd] == register_value[rs1] ^  register_value[rs2];
           
                    } else if (strcmp(tokens[0], "sll") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);
                        
                        if (register_value[rs2] <= 63 && register_value[rs2] >=0){
                            register_value[rd] == register_value[rs1] <<  register_value[rs2];
                        }

          
                    } else if (strcmp(tokens[0], "srl") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);

                        if(register_value[rs2] <= 63 && register_value[rs2] >= 0){
                            register_value[rd] = (unsigned int) register_value[rs1] >>  register_value[rs2];
                        }

           
                    } else if (strcmp(tokens[0], "sra") == 0) {
                        int rd = register_finder(tokens[1]);
                        int rs1 = register_finder(tokens[2]);
                        int rs2 = register_finder(tokens[3]);

                        if (register_value[rs2] <= 63 && register_value[rs2] >= 0){
                            register_value[rd] = register_value[rs1] >>  register_value[rs2];
                        }
           
                    } else if (strcmp(tokens[0], "addi") == 0) {
           
                    } else if (strcmp(tokens[0], "andi") == 0) {
            
                    } else if (strcmp(tokens[0], "ori") == 0) {
            
                    } else if (strcmp(tokens[0], "xori") == 0) {
            
                    } else if (strcmp(tokens[0], "slli") == 0) {
           
                    } else if (strcmp(tokens[0], "srli") == 0) {
            
                    } else if (strcmp(tokens[0], "srai") == 0) {
            
                    }
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
