#include "functions.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define MAX_ADDRESS 0x50001
typedef struct {
    uint64_t address;
    uint8_t value;
} MemEntry;

typedef struct {
    bool valid;
    bool dirty;
    unsigned int tag;
    int* data;
} cacheblock;
typedef struct {
    cacheblock* blocks;
} cachesets;
typedef struct{
    cachesets *sets;
    int numsets;
    int associativity;
    int block_size;
    int hits;
    int misses;
    char* replacement_policy;
    char* wb_policy;
} cache;
void fifo(cache* cache,unsigned int ptag,int index,int loadnumber,MemEntry* mem,uint64_t address,int* t){
     int x=0;
     cache->sets[index].blocks[x].tag = ptag;
    cache->sets[index].blocks[x].valid = 1;
    for (int k = 0; k < loadnumber; k++) {
        cache->sets[index].blocks[x].data[k] = mem[address+k].value;
    }
    *t=x;
}
void lru(cache* cache,unsigned int ptag,int index,int loadnumber,MemEntry* mem,uint64_t address,int* t){

}
void random_policy(cache* cache,unsigned int ptag,int index,int loadnumber,MemEntry* mem,uint64_t address,int* t){
     srand(time(NULL));
     int x=rand() % cache->associativity;
     cache->sets[index].blocks[x].tag = ptag;
    cache->sets[index].blocks[x].valid = 1;
    for (int k = 0; k < loadnumber; k++) {
        cache->sets[index].blocks[x].data[k] = mem[address+k].value;
    }
    *t=x;
}
void cache_read(cache *cache,unsigned int ptag,int index, long int *result, int load_number, MemEntry  *mem_entries,uint64_t address,FILE* cache_out){
     //int t;
     for(int i=0;i<cache->associativity;i++){
        //printf("check %d\n",index);
        unsigned int tagcheck=cache->sets[index].blocks[i].tag;
        if(cache->sets[index].blocks[i].tag==ptag && cache->sets[index].blocks[i].valid==1){
            //printf("hit\n");
            for(int k=0;k<load_number;k++){
                //cache->sets[index].blocks[i].data[k]!=NULL){
                    *result |=((int64_t)cache->sets[index].blocks[i].data[k]<<(k*8));
                    //printf("%ld for %d\n",*(result),cache->sets[index].blocks[i].data[k]);
                // } else {
                //     printf("error\n");
                //     break;
                // }
                
            }
            fprintf(cache_out,"R: Address: 0x%x, Set: 0x%x, Hit, Tag: 0x%x, ",address,index,ptag);
            if(cache->sets[index].blocks[i].dirty==0){
                fprintf(cache_out,"Clean\n");
            }else{
                fprintf(cache_out,"Dirty\n");
            }
            cache->hits++;
            return ;
            // for(int j=0;j<cache.block_size;j++){
            // }
        }

     }
     
     cache->misses++;
     int t;
     if(cache->associativity!=1){
        if(strcmp(cache->replacement_policy,"LRU")==0){
            //lru();
        }else if(strcmp(cache->replacement_policy,"FIFO")==0){
            fifo(cache,ptag,index,load_number,mem_entries,address,&t);
        }else if(strcmp(cache->replacement_policy,"RAND")==0){
            random_policy(cache,ptag,index,load_number,mem_entries,address,&t);
        }
        fprintf(cache_out,"R: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ",address,index,ptag);
        if(cache->sets[index].blocks[t].dirty==0){
            fprintf(cache_out,"Clean\n");
        }else{
            fprintf(cache_out,"Dirty\n");
        }
     } else {
        cache->sets[index].blocks[0].tag = ptag;
        cache->sets[index].blocks[0].valid = 1;
        for (int k = 0; k < load_number; k++) {
            cache->sets[index].blocks[0].data[k] = mem_entries[address+k].value;
            *result |=((int64_t)mem_entries[address+k].value<<(k*8));
            
        //printf("%d for cache->sets[index].blocks[0].data[%d] and mem_entries[%d+%d].value=%d\n",cache->sets[index].blocks[0].data[k],k,address,k,mem_entries[address+k].value);
        }
        fprintf(cache_out,"R: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ",address,index,ptag);
        if(cache->sets[index].blocks[0].dirty==0){
            fprintf(cache_out,"Clean\n");
        }else{
            fprintf(cache_out,"Dirty\n");
        }
     }   
     //printf("miss\n");

     
}
void run_instruction(char* line,char **tokens, long int register_value[],MemEntry  *mem_entries,int *pc_counter, char **label_names, int label_line_numbers[], int *counter_ptr,int label_position_iter,Stack* call_stack,bool cache_in,cache* cache,int cachesize,FILE* cache_out){
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
        if (cache_in == 0){
            for(int k=0;k<8;k++){
                result |=((int64_t)mem_entries[mem+k].value<<(k*8));
            }
            
        } else{
            if (cache->associativity == 1){
                int byte_offset_bits = (int)(log((double)cache->block_size)/log(2));
                int index_bits = (int)(log((double)(cachesize/cache->block_size))/log(2));
                //printf("%d %d %d %d\n",index_bits,cachesize,cache.block_size,byte_offset_bits);
                //printf("%x\n",mem);
                uint32_t c_m = mem >> (byte_offset_bits);
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                //printf("0x%x 0x%x\n",index,tag);
                cache_read(cache,tag,index,&result,8,mem_entries,mem,cache_out);
                //printf("hits=%d,misses=%d\n",cache->hits,cache->misses);
            } else if(cache->associativity==0){

            }
        }    
        // else{
        //     if (associativity == 1){
        //         int byte_offset_bits = (int)log2((double)block_size);
        //         int index_bits = (int)log2((double)max_blocks);
                
        //         uint64_t c_m = mem >> (byte_offset_bits);
        //         int tag = c_m >> (index_bits);
        //         tag = tag << (index_bits);
        //         int index = c_m - tag;
        //         bool h_m2 = 0;
        //         for (int i = 0; i < block_size; ++i){
        //             if (cache_mem[index][i].valid_bit == 1){
        //                 if (cache_mem[index][i].address == mem){
        //                     //increase hit
        //                     h_m2 = 1;
        //                     result=result|mem_entries[cache_mem[index][i].address].value;
        //                 }
        //             } else{
        //                 cache_mem[index][i].address=mem+i;
        //             }
        //         }
        //         if (h_m2 == 0){
        //             for (int i = 0; i < block_size; ++i){
        //                 cache_mem[index][i].address = mem+i;
        //             }

        //         }
        //         //cache_mem[im][i].address=mem+i
        //     } else if(associativity==0){
                   
        //     } else {

        //     }   
        // }
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
