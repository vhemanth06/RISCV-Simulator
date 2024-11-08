#ifndef RUN_H
#define RUN_H
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "stack.h"
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
    int set;
} cache_block;
void run_instruction(char* line,char **tokens, long int register_value[],MemEntry  *mem_entries,int *pc_counter, char **label_names, int label_line_numbers[], int *counter_ptr,int label_position_iter,Stack* call_stack,cache_block** cache_mem, bool cache_in, int associativity, int block_size,int max_blocks,char* replace_pol, char* wb_policy);
#endif