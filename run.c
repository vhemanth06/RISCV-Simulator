#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define MAX_ADDRESS 0x50000
typedef struct {
    uint64_t address;
    uint8_t value;
} MemEntry;

void run_instruction(char **tokens, long int register_value[],MemEntry  *mem_entries){
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
            register_value[rd] = (uint64_t) register_value[rs1] >> num;
        }
            
    } else if (strcmp(tokens[0], "srai") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int num = atoi(tokens[3]);

        if (num <= 63 && num >= 0){
            register_value[rd] = (int64_t)register_value[rs1] >> num;
        }
            
    } else if(strcmp(tokens[0], "lui") == 0){
        int rd = register_finder(tokens[1]);
        char *endptr;
        long int num = strtol(tokens[2], &endptr, 0);

        register_value[rd] = num<<12;

    } else if (strcmp(tokens[0], "ld") == 0) {
        printf("runcheck\n");
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int64_t result = 0;
        uint64_t mem  = (uint64_t)(register_value[rs1] + num);
        printf("runcheck\n");
        for(int k=0;k<8;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
        printf("%ld\n",result);    
    } else if (strcmp(tokens[0], "lw") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int32_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for(int k=0;k<4;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
        printf("%d\n",result); 
    } else if (strcmp(tokens[0], "lh") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int16_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for(int k=0;k<2;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
    } else if (strcmp(tokens[0], "lb") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int8_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for(int k=0;k<1;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
    } else if (strcmp(tokens[0], "lwu") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint32_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for(int k=0;k<4;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
    } else if (strcmp(tokens[0], "lhu") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint16_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for(int k=0;k<2;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
    } else if (strcmp(tokens[0], "lbu") == 0){
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint8_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for(int k=0;k<1;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
    } else if (strcmp(tokens[0], "sd") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<8; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        
    } else if (strcmp(tokens[0], "sw") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<4; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        
    } else if (strcmp(tokens[0], "sh") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<2; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        
    } else if (strcmp(tokens[0], "sb") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]); 
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<1; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        
    }
}