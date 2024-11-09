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
// typedef struct cache_block{
//     bool H_M;
//     bool dirty;
//     bool valid_bit;
//     uint64_t address ;
//     uint8_t value;
//     int set;
// } cache_block;
typedef struct {
    bool valid;
    unsigned int tag;
    int* data;
} cacheblock;
typedef struct {
    cacheblock* blocks;
} cachesets;
typedef struct{
    cachesets* sets;
    int numsets;
    int associativity;
    int block_size;
    int hits;
    int misses;
    char* replacement_policy;
    char* wb_policy;
} cache;
void run_instruction(char* line,char **tokens, long int register_value[],MemEntry  *mem_entries,int *pc_counter, char **label_names, int label_line_numbers[], int *counter_ptr,int label_position_iter,Stack* call_stack);
#endif