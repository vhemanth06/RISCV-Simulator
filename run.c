#include "functions.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define MAX_ADDRESS 0x50001
typedef struct {
    uint64_t address;
    uint8_t value;
} MemEntry;

typedef struct cache_block{
    bool H_M;
    bool dirty;
    bool valid_bit;
    uint64_t address ;
    uint8_t value;
} cache_block;

void run_instruction(char* line,char **tokens, long int register_value[],MemEntry  *mem_entries,int *pc_counter, char **label_names, int label_line_numbers[], int *counter_ptr,int label_position_iter,Stack* call_stack,cache_block** cache_mem){
    if (strcmp(tokens[0], "add") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] +  register_value[rs2];
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
            
    } else if (strcmp(tokens[0], "sub") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] -  register_value[rs2];
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;   
    } else if (strcmp(tokens[0], "and") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] &  register_value[rs2];
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;  
    } else if (strcmp(tokens[0], "or") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] |  register_value[rs2];
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;  
    } else if (strcmp(tokens[0], "xor") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] ^  register_value[rs2];
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;   
    } else if (strcmp(tokens[0], "sll") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);
            register_value[rd] = register_value[rs1] <<  register_value[rs2];
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter=*pc_counter+4;
        
        
    } else if (strcmp(tokens[0], "srl") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);
            register_value[rd] = (unsigned int) register_value[rs1] >>  register_value[rs2];
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
                  
    } else if (strcmp(tokens[0], "sra") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);
           register_value[rd] = register_value[rs1] >>  register_value[rs2];
           call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
          
    } else if (strcmp(tokens[0], "addi") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char* endpointer;
        long int num = strtol(tokens[3],&endpointer,0);
            register_value[rd] = register_value[rs1] + num;
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
        
        
    } else if (strcmp(tokens[0], "andi") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char* endpointer;
        long int num = strtol(tokens[3],&endpointer,0);
            register_value[rd] = register_value[rs1] & num;
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
        
            
    } else if (strcmp(tokens[0], "ori") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char* endpointer;
        long int num = strtol(tokens[3],&endpointer,0);
            register_value[rd] = register_value[rs1] | num;
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
        
            
    } else if (strcmp(tokens[0], "xori") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char* endpointer;
        long int num = strtol(tokens[3],&endpointer,0);
            register_value[rd] = register_value[rs1] ^ num;
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
        
           
    } else if (strcmp(tokens[0], "slli") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char* endpointer;
        long int num = strtol(tokens[3],&endpointer,0);
            register_value[rd] = register_value[rs1] << num;
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4; 
        
          
    } else if (strcmp(tokens[0], "srli") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char* endpointer;
        long int num = strtol(tokens[3],&endpointer,0);
            register_value[rd] = (uint64_t) register_value[rs1] >> num;
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
        
           
    } else if (strcmp(tokens[0], "srai") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char* endpointer;
        long int num = strtol(tokens[3],&endpointer,0);
            register_value[rd] = (int64_t)register_value[rs1] >> num;
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4; 
        
           
    } else if(strcmp(tokens[0], "lui") == 0){
        int rd = register_finder(tokens[1]);
        char *endptr;
        int num = strtol(tokens[2], &endptr, 0) & 0xfffff;

        register_value[rd] = (long int)(num<<12);
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
    } else if (strcmp(tokens[0], "ld") == 0) {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int64_t result = 0;
        uint64_t mem  = (uint64_t)(register_value[rs1] + num);
        for(int k=0;k<8;k++){
            result |=((int64_t)mem_entries[mem+k].value<<(k*8));
        }
        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
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
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
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
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
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
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
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
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
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
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
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
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
    } else if (strcmp(tokens[0], "sd") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<8; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
    } else if (strcmp(tokens[0], "sw") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<4; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
    } else if (strcmp(tokens[0], "sh") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<2; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
    } else if (strcmp(tokens[0], "sb") == 0){
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]); 
        int num = atoi(tokens[2]); 
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        for (int i = 0; i<1; i++){
            mem_entries[mem+i].value=(register_value[rs2] >> (i * 8)) & 0xFF;
        }
        call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        *pc_counter=*pc_counter+4;
    } else if (strcmp(tokens[0], "beq") == 0){
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index=-1;
        int imm;

        for(int k=0;k<label_position_iter;k++){
                   if(strcmp(tokens[3],label_names[k])==0){
                        index=label_line_numbers[k];
                        break;
                    }
        }
        if (index!=-1){
                imm=((index)-(*counter_ptr))*4;
            }  
        if(index == -1 && non_int_char_finder(tokens[3])==0 ){
                imm=atoi(tokens[3]);
            }
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        if (register_value[rs1] == register_value[rs2]){
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm/4);
            (*counter_ptr)--;
        }
        else {
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    } else if (strcmp(tokens[0], "bne") == 0){
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index=-1;
        int imm;

        for(int k=0;k<label_position_iter;k++){
                   if(strcmp(tokens[3],label_names[k])==0){
                        index=label_line_numbers[k];
                        break;
                    }
        }
        if (index!=-1){
                imm=((index)-(*counter_ptr))*4;
            }  
        if(index == -1 && non_int_char_finder(tokens[3])==0 ){
                imm=atoi(tokens[3]);
            }
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        if (register_value[rs1] != register_value[rs2]){
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm/4);
            (*counter_ptr)--;
        }
        else {
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    } else if (strcmp(tokens[0], "blt") == 0){

        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index=-1;
        int imm;

        for(int k=0;k<label_position_iter;k++){
                   if(strcmp(tokens[3],label_names[k])==0){
                        index=label_line_numbers[k];
                        break;
                    }
        }
        if (index!=-1){
                imm=((index)-(*counter_ptr))*4;
            }  
        if(index == -1 && non_int_char_finder(tokens[3])==0 ){
                imm=atoi(tokens[3]);
            }
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        if (register_value[rs1] < register_value[rs2]){
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm/4);
            (*counter_ptr)--;
        }
        else {
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    } else  if (strcmp(tokens[0], "bge") == 0){
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index=-1;
        int imm;

        for(int k=0;k<label_position_iter;k++){
                   if(strcmp(tokens[3],label_names[k])==0){
                        index=label_line_numbers[k];
                        break;
                    }
        }
        if (index!=-1){
                imm=((index)-(*counter_ptr))*4;
            }  
        if(index == -1 && non_int_char_finder(tokens[3])==0 ){
                imm=atoi(tokens[3]);
            }
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        if (register_value[rs1] >= register_value[rs2]){
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm/4);
            (*counter_ptr)--;
        }
        else {
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    } else  if (strcmp(tokens[0], "bltu") == 0){
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index=-1;
        int imm;

        for(int k=0;k<label_position_iter;k++){
                   if(strcmp(tokens[3],label_names[k])==0){
                        index=label_line_numbers[k];
                        break;
                    }
        }
        if (index!=-1){
                imm=((index)-(*counter_ptr))*4;
            }  
        if(index == -1 && non_int_char_finder(tokens[3])==0 ){
                imm=atoi(tokens[3]);
            }
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        if ((unsigned int)register_value[rs1] < (unsigned int)register_value[rs2]){
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm/4);
            (*counter_ptr)--;
        }
        else {
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    } else  if (strcmp(tokens[0], "bgeu") == 0){
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index=-1;
        int imm;

        for(int k=0;k<label_position_iter;k++){
                   if(strcmp(tokens[3],label_names[k])==0){
                        index=label_line_numbers[k];
                        break;
                    }
        }
        if (index!=-1){
                imm=((index)-(*counter_ptr))*4;
            }  
        if(index == -1 && non_int_char_finder(tokens[3])==0 ){
                imm=atoi(tokens[3]);
            }
            call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
        if ((unsigned int)register_value[rs1] >= (unsigned int)register_value[rs2]){
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm/4);
            (*counter_ptr)--;
        }
        else {
            printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    } else if (strcmp(tokens[0], "jal") == 0){
        int rs1 = register_finder(tokens[1]);
        int index = -1;
        int imm;
        for(int k=0;k<label_position_iter;k++){
            if(strcmp(tokens[2],label_names[k])==0){
                index=label_line_numbers[k];
                call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
                push(call_stack,tokens[2]);
                call_stack->line_num[call_stack->top_index]=*counter_ptr+1;
                break;
            }
        }
        if (index != -1){
            imm=((index)-(*counter_ptr))*4;
            

        }
        if(index == -1 && non_int_char_finder(tokens[3])==0 ){
            imm=atoi(tokens[3]);
        }
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        register_value[rs1] = *pc_counter + 4;
        *pc_counter = *pc_counter + imm;
        *counter_ptr = *counter_ptr + (imm/4);
        (*counter_ptr)--;
    } else if (strcmp(tokens[0], "jalr") == 0){
        int rs2 = register_finder(tokens[3]);
        int rs1=register_finder(tokens[1]);
        int imm= atoi(tokens[1]);
        int pc_1 = *pc_counter;
        printf("Executed %s; PC=0x%08x\n",line,*pc_counter);
        register_value[rs1] = *pc_counter+4;
        *pc_counter = register_value[rs2]+imm;
        int diff;
        diff = *pc_counter - pc_1;
        pop(call_stack);
        *counter_ptr = *counter_ptr + (diff/4);
        (*counter_ptr)--;
    }
}
