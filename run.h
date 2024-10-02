#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "stack.h"
typedef struct {
    uint64_t address;
    uint8_t value;
} MemEntry;
void run_instruction(char* line,char **tokens, long int register_value[],MemEntry  *mem_entries,int *pc_counter, char **label_names, int label_line_numbers[], int *counter_ptr,int label_position_iter,Stack* call_stack);