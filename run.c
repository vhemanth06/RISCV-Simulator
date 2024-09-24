#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void run_instruction(char **tokens, long int register_value[],uint64_t mem_address[],int64_t mem_values[]){
    if (strcmp(tokens[0], "add") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] +  register_value[rs2];
        //printf("%d+%d=%d\n",register_value[rs1],register_value[rs2],register_value[rd]);
            
    } else if (strcmp(tokens[0], "sub") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] -  register_value[rs2];
           
    } else if (strcmp(tokens[0], "and") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] &  register_value[rs2];
            
    } else if (strcmp(tokens[0], "or") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] |  register_value[rs2];
           
    } else if (strcmp(tokens[0], "xor") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] ^  register_value[rs2];
           
    } else if (strcmp(tokens[0], "sll") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);
                        
        if (register_value[rs2] <= 63 && register_value[rs2] >=0){
            register_value[rd] = register_value[rs1] <<  register_value[rs2];
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
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 1023 && num >= -1024){
            register_value[rd] = register_value[rs1] + num;
        }

    } else if (strcmp(tokens[0], "andi") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 1023 && num >= -1024){
            register_value[rd] = register_value[rs1] & num;
        }
            
    } else if (strcmp(tokens[0], "ori") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 1023 && num >= -1024){
            register_value[rd] = register_value[rs1] | num;
        }
            
    } else if (strcmp(tokens[0], "xori") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 1023 && num >= -1024){
            register_value[rd] = register_value[rs1] ^ num;
        }
            
    } else if (strcmp(tokens[0], "slli") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 63 && num >= 0){
            register_value[rd] = register_value[rs1] << num;
        }
           
    } else if (strcmp(tokens[0], "srli") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 63 && num >= 0){
            register_value[rd] = (unsigned int) register_value[rs1] >> num;
        }
            
    } else if (strcmp(tokens[0], "srai") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 63 && num >= 0){
            register_value[rd] = register_value[rs1] >> num;
        }
            
    } else if(strcmp(tokens[0], "lui") == 0){
        int rd = register_finder(tokens[1]);
        char *endptr;
        long int num = strtol(tokens[2], &endptr, 0);

        register_value[rd] = num;

    } else if (strcmp(tokens[0], "ld") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int found = 0;
        int64_t result = 0;

        uint64_t mem  = (uint64_t)(register_value[rs1] + num);

        for (int i = 0; i < 50000; i++){
            if (mem_address[i] == mem){
                found = 1;
                for (int k = 0; k < 8; k++){
                    result |= (mem_values[i + k] << (k * 8)); 
                }
                register_value[rd] = result;
                break;
            }
        }
        if(found != 1){
            register_value[rd] = 0;
        }
            
    } else if (strcmp(tokens[0], "lw") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int found = 0;
        int32_t result = 0;

        uint64_t mem = (uint64_t)(register_value[rs1] + num);

        for (int i = 0; i < 50000; i++){
            if (mem_address[i] == mem){
                found = 1;
                for (int k = 0; k < 4; k++){
                    result |= (mem_values[i + k] << (k * 8));
                }
                register_value[rd] = result;
                break;
            }
        }
        if (found != 1){
            register_value[0] = 0;
        }
    } else if (strcmp(tokens[0], "lh") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int found = 0;
        int16_t result = 0;

        uint64_t mem = (uint64_t)(register_value[rs1] + num);

        for (int i = 0; i < 50000; i++){
            if (mem_address[i] == mem){
                found = 1;
                for (int k = 0; k < 2; k++){
                    result |= (mem_values[i + k] << (k * 8));
                }
                register_value[rd] = result;
                break;
            }
        }
        if (found != 1){
            register_value[0] = 0;
        }
    } else if (strcmp(tokens[0], "lb") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int found = 0;
        int8_t result = 0;

        uint64_t mem = (uint64_t)(register_value[rs1] + num);

        for (int i = 0; i < 50000; i++){
            if (mem_address[i] == mem){
                found = 1;
                for (int k = 0; k < 1; k++){
                    result |= (mem_values[i + k] << (k * 8));
                }
                register_value[rd] = result;
                break;
            }
        }
        if (found != 1){
            register_value[0] = 0;
        }
    }
}