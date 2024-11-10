#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "run.h"
#include "stack.h"
#include <stdint.h>
#define MAX_ADDRESS 0x50001
#define MAX_TOKENS 50
#define MAX_INPUT_SIZE 100 
#define MAX_LINES 500
//#define MAX_CONFIG_LINES 5
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
int *breakpoint;
bool cache_in=0;

int main() {
    mem_entries = malloc(MAX_ADDRESS * sizeof(MemEntry));
    if (mem_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    char command[MAX_INPUT_SIZE];
    int stepper = 0;
    int *counter_ptr;
    int *stepper_ptr;
    int counter = 0;
    counter_ptr = &counter;
    stepper_ptr = &stepper;
     int i=0;
     int tnum=0;
     char line[MAX_INPUT_SIZE];
    char *array_of_lines[MAX_LINES];
    Stack* call_stack;
    cache cache;
    int cache_size;
    int numblocks;
    FILE* cache_out;
    
    
    while (1) {  
        fgets(command,sizeof(command),stdin);   
        FILE *input;   
        char **tokens_comm = string_split(command);
           if(strcmp(tokens_comm[0], "load") == 0){
            pc_counter=0;
            counter=0;
            input = fopen(tokens_comm[1], "r");
            if(cache_in==1){
                char* filename=strdup(tokens_comm[1]);
                //printf("%s\n",tokens_comm[1]);
                //strcpy(filename,tokens_comm[1]);
                char *dot = strrchr(filename, '.');
                if (dot != NULL) {
                    strcpy(dot, ".out");
                } else {
                    strcat(filename, ".out");
                }
                cache_out=fopen(filename,"w");
                cache.hits=0;
                cache.misses=0;
                if(cache.associativity!=0){
                    for (int i = 0; i < cache.numsets; i++) {
                        for(int j=0;j<cache.associativity;j++){
                            cache.sets[i].blocks[j].valid=0;
                            cache.sets[i].blocks[j].tag=0;
                            cache.sets[i].blocks[j].dirty=0;
                        }    
                    }
                } else {
                    for (int i = 0; i < numblocks; i++) {
                        cache.sets[0].blocks[i].valid = 0;  
                        cache.sets[0].blocks[i].tag = 0;
                        cache.sets[0].blocks[i].dirty=0;    
                    }
                }
            }
            memcpy(register_value,default_register_value,sizeof(register_value));
            if(input==NULL){
                printf("error opening file\n");
                fclose(input);
            }
            for(int k=0;k<MAX_ADDRESS;k++){
                mem_entries[k].address=k;
                mem_entries[k].value=0;
            }
            
                for (int k = 0; k < MAX_LINES; k++) {
                     array_of_lines[k] = malloc(MAX_INPUT_SIZE * sizeof(char));
                    if (array_of_lines[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for line %d\n", k);
                    return 1; 
                    }
                }
               
            while(fgets(line,sizeof(line),input)!=NULL){
                    char *substrings = strtok(line,"\n\r"); 
                    if(substrings!=NULL){
                       strcpy(array_of_lines[i],line);
                       i++;
                    }
                    if(i>=MAX_LINES){
                        break;
                    }
                }
                breakpoint=(int*)malloc((i+1)*sizeof(int));
                for(int k=0;k<i+1;k++){
                    breakpoint[i]=0;
                }
                rewind(input);
                while(fgets(line,sizeof(line),input)!=NULL){
                    if(line[0]=='.'){
                        tnum++;
                    }
                }
                rewind(input);
                call_stack=createEmptyStack();
                char* string=strdup("main");
                push(call_stack,string);
                call_stack->line_num[call_stack->top_index]=tnum;
                printf("\n");
            continue;
           } else if(strcmp(tokens_comm[0],"run")==0){
                if (input == NULL) {
                    printf("input file not found\n");
                    fclose(input);
                }
                char line[MAX_INPUT_SIZE];
                char *array_of_lines[MAX_LINES];
                for (int k = 0; k < MAX_LINES; k++) {
                     array_of_lines[k] = malloc(MAX_INPUT_SIZE * sizeof(char));
                    if (array_of_lines[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for line %d\n", k);
                    return 1; 
                    }
                }
                uint64_t d_address=0x10000;
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
                i=0;
                rewind(input);
                while(fgets(line,sizeof(line),input)!=NULL){
                    char *substrings = strtok(line,"\n\r"); 
                    if(substrings!=NULL){
                       strcpy(array_of_lines[i],line);
                       i++;
                    }
                    if(i>=MAX_LINES){
                        break;
                    }
                }
                rewind(input);
                while(counter < i){
                    int size=strlen(array_of_lines[counter]);
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
                    instruction=trim_space(instruction);
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
                    if(strcmp(tokens[0],".data")==0 || strcmp(tokens[0],".text")==0){
                        counter++;
                        continue;
                    }
                    if(strcmp(tokens[0],".dword")==0){
                         if(tokens[1]!=NULL){
                            for(int k=1;tokens[k]!=NULL;k++){
                                char* endpointer;
                                long int num = strtol(tokens[k],&endpointer,0);
                                int m = 0;
                                for ( m = 0; m < 8; m++) {
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address=d_address+8;  
                            }
                        }
                         counter++;
                    } else if(strcmp(tokens[0],".word")==0){
                        if (tokens[1] != NULL){
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 4; m++){
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address = d_address + 4;
                            }
                        }
                        counter++;
                    }  else if(strcmp(tokens[0],".half")==0){
                        if (tokens[1] != NULL){
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 2; m++){
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address = d_address + 2;
                            }
                        }
                        counter++;
                    }  else if(strcmp(tokens[0],".byte")==0){
                        if (tokens[1] != NULL){
                            for (int k = 1; tokens[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(tokens[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 1; m++){
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address = d_address + 1;
                            }
                        }
                        counter++;
                    }
                    run_instruction(instruction_copy,tokens, register_value,mem_entries,&pc_counter,
                              label_names, label_line_numbers, counter_ptr,label_position_iter,call_stack,cache_in,&cache,cache_size,cache_out);
                    register_value[0]=0;
                    if(counter==i-1){
                        pop(call_stack);
                    }
                    if(counter==i-1 && cache_in==1){
                        printf("D-cache statistics: Accesses=%d, Hit=%d, Miss=%d, Hit Rate=%.2f",cache.hits+cache.misses,cache.hits,cache.misses,
                  (float)cache.hits/(cache.hits+cache.misses));
                    }
                    counter++;
                    if (breakpoint[counter+1]==1){
                         printf("Execution stopped at breakpoint\n");
                         break;
                    }
                }
                printf("\n");
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
                printf("Exited the simulator\n");
                free(mem_entries);
                return 0;
           } else if(strcmp(tokens_comm[0],"mem")==0){
            char* endpointer;
                uint64_t mem=strtoul(tokens_comm[1],&endpointer,16);
                int count=atoi(tokens_comm[2]);
                for(int k=0;k<count;k++){
                    printf("Memory[0x%05lX] = 0x%02X\n",mem_entries[mem+k].address,mem_entries[mem+k].value);
                }
                printf("\n");
           } else if (strcmp(tokens_comm[0], "step") == 0){
                stepper = counter;
                if(stepper >= i){
                    printf("Nothing to step\n\n");
                    continue;
                }
                if (input == NULL) {
                    printf("input file not found\n");
                    fclose(input);
                }
                char line[MAX_INPUT_SIZE];
                char line2[MAX_INPUT_SIZE];
                char *array_of_lines[MAX_LINES];
                for (int k = 0; k < MAX_LINES; k++) {
                     array_of_lines[k] = malloc(MAX_INPUT_SIZE * sizeof(char));
                    if (array_of_lines[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for line %d\n", k);
                    return 1; 
                    }
                }
                int i=0;
                uint64_t d_address=0x10000;
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
                while (fgets(line2, sizeof(line2), input) != NULL && stepper == 0){
                    char *pieces[100];
                    char *token;
                    int i = 0;

                    int len = strlen(line2);
                    if (len > 0 && line2[len - 1] == '\n') {
                        line2[len - 1] = '\0';
                    }
                    for (int i = 0; line2[i]; ++i) {
                        if (!isprint(line2[i])) {
                            line2[i] = ' ';
                        }
        
                        if (line2[i] == ':') {
                            for (int j = 0; j < i + 1; ++j) {
                                line2[j] = ' ';
                        }
                        } else if (line2[i] == ',' || line2[i] == '(' || line2[i] == ')') {
                            line2[i] = ' ';
                        }
                    }
                    token = strtok(line2, " ");
                    while (token != NULL && i < 100) {
                    pieces[i] = token;
                    i++;
                    token = strtok(NULL, " ");
                    }
                    pieces[i] = NULL;

                    if (strcmp(pieces[0], ".data") == 0 || strcmp(pieces[0] ,".text") == 0){
                        continue;
                    }
                    else if(strcmp(pieces[0],".dword")==0){
                         if(pieces[1]!=NULL){
                            for(int k=1;pieces[k]!=NULL;k++){
                                char* endpointer;
                                long int num = strtol(pieces[k],&endpointer,0);
                                int m = 0;
                                for ( m = 0; m < 8; m++) {
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address=d_address+8;  
                            }
                        }
                    } else if(strcmp(pieces[0],".word")==0){
                        if (pieces[1] != NULL){
                            for (int k = 1; pieces[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(pieces[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 4; m++){
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address = d_address + 4;
                            }
                        }
                    }  else if(strcmp(pieces[0],".half")==0){
                        if (pieces[1] != NULL){
                            for (int k = 1; pieces[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(pieces[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 2; m++){
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address = d_address + 2;
                            }
                        }
                    }  else if(strcmp(pieces[0],".byte")==0){
                        if (pieces[1] != NULL){
                            for (int k = 1; pieces[k] != NULL; k++){
                                char *endpointer;
                                long int num = strtol(pieces[k], &endpointer, 0);
                                int m = 0;
                                for (m = 0; m < 1; m++){
                                    mem_entries[d_address+m].value = (num >> (m * 8)) & 0xFF;
                                }
                                d_address = d_address + 1;
                            }

                        }
                    } else{break;}
                }
                rewind(input);
                while(fgets(line,sizeof(line),input)!=NULL){
                    char *substrings = strtok(line,"\n\r"); 
                    if(substrings!=NULL){
                       strcpy(array_of_lines[i],line);
                       i++;
                    }
                    if (stepper == 0){
                        if (strcmp(substrings, ".text") == 0) {
                            stepper = i; 
                            continue; 
                        }
                    }
                    if(i>=MAX_LINES){
                        break;
                    }
                }
                rewind(input);
                int size=strlen(array_of_lines[stepper]);
                char *instruction;
                    char *label;
                    char line_copy1[size];
                    strcpy(line_copy1,array_of_lines[stepper]);
                    char *copy=deepCopyString(array_of_lines[stepper]);
                    char *tokens_for_labels = strtok(copy, ":\n\r");
                    
                    if(ischarinstring(line_copy1,':')==1){
                        label = tokens_for_labels;
                        tokens_for_labels = strtok(NULL, ":\n\r");
                        instruction = tokens_for_labels;
                    } else {
                        instruction = tokens_for_labels;
                        label=NULL;
                    }
                    instruction=trim_space(instruction);
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
                    run_instruction(instruction_copy,tokens, register_value,mem_entries,&pc_counter, label_names, label_line_numbers, stepper_ptr,label_position_iter,call_stack,cache_in,&cache,cache_size,cache_out);
                    register_value[0]=0;
                    if(stepper==i-1){
                        pop(call_stack);
                    }
                    if(stepper==i-1 && cache_in==1){
                        printf("D-cache statistics: Accesses=%d, Hit=%d, Miss=%d, Hit Rate=%.2f",cache.hits+cache.misses,cache.hits,cache.misses,
                  (float)cache.hits/(cache.hits+cache.misses));
                    }
                    stepper++; 
                    counter = stepper;
                    printf("\n");     
           } else if (strcmp(tokens_comm[0], "break") == 0){
                int num = atoi(tokens_comm[1]);
                if(num<=i){
                    breakpoint[num]=1;
                    printf("Breakpoint set at line %d\n", num);
                }
                printf("\n");
                
           } else if (strcmp(tokens_comm[0], "del") == 0){
                int num = atoi(tokens_comm[2]);
                if(breakpoint[num]==1){
                   breakpoint[num]=0;
                   printf("Breakpoint removed at line %d\n", num);
                } else {
                    printf("No breakpoint exits\n");
                }
                printf("\n");
                
           } else if(strcmp(tokens_comm[0], "show-stack") == 0){
                  if(call_stack->top_index==-1){
                    printf("Empty Call Stack: Execution complete\n");
                  } else {
                    printf("Call Stack:\n");
                    for (int i=0;i<=call_stack->top_index;i++){
                        printf("%s:%d\n",call_stack->value[i],call_stack->line_num[i]);
                    }
                  }
                  printf("\n");
           } else if(strcmp(tokens_comm[0],"cache_sim")==0 && strcmp(tokens_comm[1],"enable")==0){
                  cache_in=1;
                  FILE* config;
                  config=fopen(tokens_comm[2],"r");
                  char config_line[MAX_INPUT_SIZE];
                  char *array_for_config[5];
                for (int k = 0; k < 5; k++) {
                     array_for_config[k] = malloc(MAX_INPUT_SIZE * sizeof(char));
                    if (array_for_config[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for line %d\n", k);
                    return 1; 
                    }
                }
                  int p=0;
                  while(fgets(config_line,sizeof(config_line),config)!=NULL){
                     char *substrings2 = strtok(config_line,"\n\r"); 
                    if(substrings2!=NULL){
                       strcpy(array_for_config[p] ,config_line);
                       p++;
                       //printf("%s\n",array_for_config[p-1]);
                    }
                  }
                  cache_size=atoi(array_for_config[0]);
                  //printf("%d\n",cache_size);
                  cache.block_size=atoi(array_for_config[1]);
                  
                  cache.associativity=atoi(array_for_config[2]);
                  cache.replacement_policy=strdup(array_for_config[3]);
                  cache.wb_policy=strdup(array_for_config[4]);
                  numblocks=cache_size/cache.block_size;
                  cache.hits=0;
                  cache.misses=0;
                  if(cache.associativity!=0){
                    cache.numsets=numblocks/cache.associativity;
                    cache.sets = malloc(cache.numsets * sizeof(cachesets));
                    for (int i = 0; i < cache.numsets; i++) {
                        cache.sets[i].blocks = malloc(cache.associativity * sizeof(cacheblock));
                        cache.sets[i].fifo_queue = malloc(cache.associativity * sizeof(int));  // FIFO queue for tracking block order

        // Initialize FIFO queue (set it to -1, indicating an empty state)
                    for (int j = 0; j < cache.associativity; j++) {
                        cache.sets[i].fifo_queue[j] = -1;  // No blocks are placed yet
                    }
                        for(int j=0;j<cache.associativity;j++){
                            cache.sets[i].blocks[j].data=malloc(cache.block_size*sizeof(int));
                            cache.sets[i].blocks[j].valid=0;
                            cache.sets[i].blocks[j].tag=0;
                            cache.sets[i].blocks[j].dirty=0;
                        }    
                    }  
                  } else{
                    cache.numsets = 1;  
                    cache.sets = malloc(cache.numsets * sizeof(cachesets));
                    cache.sets[0].blocks = malloc(numblocks * sizeof(cacheblock)); 
                    cache.sets[0].fifo_queue = malloc(numblocks * sizeof(int));  
                    for (int j = 0; j < numblocks; j++) {
                        cache.sets[0].fifo_queue[j] = -1;  // No blocks are placed yet
                    }
                    for (int i = 0; i < numblocks; i++) {
                        cache.sets[0].blocks[i].data = malloc(cache.block_size * sizeof(int)); // Allocate memory for block data
                        cache.sets[0].blocks[i].valid = 0;  
                        cache.sets[0].blocks[i].tag = 0;
                        cache.sets[0].blocks[i].dirty=0;    
                    }
                  }  
                    printf("\n");
           } else if(strcmp(tokens_comm[0],"cache_sim")==0 && strcmp(tokens_comm[1],"disable")==0){
                    if(cache_in==1){
                        if(cache.associativity!=0){
                        for (int i = 0; i < cache.numsets; i++) {
                            for(int j=0;j<cache.associativity;j++){
                                free(cache.sets[i].blocks[j].data);
                            }
                            free(cache.sets[i].blocks); 
                            free(cache.sets[i].fifo_queue);   
                        }
                        free(cache.sets);
                        }else{
                            for(int j=0;j<numblocks;j++){
                                free(cache.sets[0].blocks[j].data);
                            }
                            free(cache.sets[0].blocks);
                            free(cache.sets[0].fifo_queue);
                            free(cache.sets);
                        }
                        cache_in=0;
                    } else {
                        printf("Error!cache simulations is disabled\n");
                    }
                    printf("\n");
           } else if(strcmp(tokens_comm[0],"cache_sim")==0 && strcmp(tokens_comm[1],"status")==0){
                if(cache_in==1){
                    printf("status:enabled\n");
                    printf("Cache Size: %d\n",cache_size);
                    printf("Block Size: %d\n",cache.block_size);
                    printf("Associativity: %d\n",cache.associativity);
                    printf("Replacement Policy: %s\n",cache.replacement_policy);
                    printf("Write Back Policy: %s\n",cache.wb_policy);
                } else {
                    printf("status:disabled\n");
                } 
                printf("\n");   
           } else if(strcmp(tokens_comm[0],"cache_sim")==0 && strcmp(tokens_comm[1],"invalidate")==0){
                    if(cache_in==1){
                        if(cache.associativity!=0){
                            for (int i = 0; i < cache.numsets; i++) {
                                for(int j=0;j<cache.associativity;j++){
                                    cache.sets[i].blocks[j].valid=0;
                                }    
                            }
                        } else {
                            for(int j=0;j<numblocks;j++){
                                    cache.sets[0].blocks[j].valid=0;
                                }
                        }
                   } else{
                     printf("Error!cache simulations is disabled\n");
                   }
                   printf("\n");
           } else if(strcmp(tokens_comm[0],"cache_sim")==0 && strcmp(tokens_comm[1],"dump")==0){
                   
                   if(cache_in==1){
                        FILE* output=fopen(tokens_comm[2],"w");

                        if(cache.associativity!=0){
                            for (int i = 0; i < cache.numsets; i++) {
                                for(int j=0;j<cache.associativity;j++){
                                    if(cache.sets[i].blocks[j].valid==1){
                                        fprintf(output,"Set: 0x%x, Tag: 0x%x, ",i,cache.sets[i].blocks[j].tag);
                                        if(cache.sets[i].blocks[j].dirty==0){
                                            fprintf(output,"Clean\n");
                                        } else {
                                            fprintf(output,"Dirty\n");
                                        }
                                    }  
                                }    
                            }
                        } else {
                            for(int j=0;j<numblocks;j++){
                                if(cache.sets[0].blocks[j].valid==1){
                                    fprintf(output,"Set: 0x0, Tag: 0x%x, ",cache.sets[0].blocks[j].tag);
                                    if(cache.sets[0].blocks[j].dirty==0){
                                        fprintf(output,"Clean\n");
                                    } else {
                                        fprintf(output,"Dirty\n");
                                    }
                                }  
                            }
                        }
                   } else{
                     printf("Error!cache simulations is disabled\n");
                   }
                   printf("\n");
                   
           } else if(strcmp(tokens_comm[0],"cache_sim")==0 && strcmp(tokens_comm[1],"stats")==0){
        //             int accesses=cache.hits+cache.misses;
        //             float hit_rate=cache.hits/accesses;
                if(cache_in==1){
                    printf("D-cache statistics: Accesses=%d, Hit=%d, Miss=%d, Hit Rate=%.2f",cache.hits+cache.misses,cache.hits,cache.misses,(float)cache.hits/(cache.hits+cache.misses));
                } else {
                    printf("Error!cache simulations is disabled\n");
                }
         } else {
            printf("Wrong input\n");
            printf("\n");
           }
    }
    free(mem_entries);
    return 0;
}
