#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
typedef struct {
    uint64_t address;
    uint8_t value;
} MemEntry;
void run_instruction(char **tokens, long int register_value[],MemEntry  *mem_entries,int j);