#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#include "run.h"
#include <stdint.h> 
#include "hex.h"

// #define MAX_ADDRESS 0x50000
// #define MAX_TOKENS 50
// #define MAX_INPUT_SIZE 100 
// #define MAX_LINES 0x50000 
//newly defined struct varaibles for every format type
// typedef struct {
//     int funct7;
//     int rs2;
//     int rs1;
//     int funct3;
//     int rd;
//     int opcode;
// } R_format_type;

R_format_type create_r_format() {
    R_format_type x = {0};
    x.opcode = 0b0110011;
    return x;
}

// typedef struct {
//     int immediate;
//     int rs1;
//     int funct3;
//     int rd;
//     int opcode;
// } I_format_type;

// typedef struct {
//     int imm_11_6;
//     int imm_5_0;
//     int rs1;
//     int funct3;
//     int rd;
//     int opcode;
// } I_format_type_exception;

// typedef struct {
//     int imm_11_5;
//     int rs2;
//     int rs1;
//     int funct3;
//     int imm_4_0;
//     int opcode;
//     int immediate;
// } S_format_type;

// typedef struct {
//     int imm_12;
//     int imm_10_5;
//     int rs2;
//     int rs1;
//     int funct3;
//     int imm_4_1;
//     int imm_11;
//     int opcode;
//     int immediate;
// } B_format_type;

// typedef struct {
//     int imm_20;
//     int imm_10_1;
//     int imm_11;
//     int imm_19_12;
//     int rd;
//     int opcode;
//     int immediate;
// } J_format_type;

// typedef struct {
//     unsigned long long int imm_31_12;
//     unsigned long long int rd;
//     unsigned long long int opcode;
//     unsigned long long int immediate;
// } U_format_type;
//Machine code caculation functions
unsigned int r_format_hex(R_format_type x) {
    return x.opcode | (x.rd << 7) | (x.funct3 << 12) | (x.rs1 << 15) | (x.rs2 << 20) | (x.funct7 << 25);
}

unsigned int I_format_hex(I_format_type x) {
    return x.opcode | (x.rd << 7) | (x.funct3 << 12) | (x.rs1 << 15) | (x.immediate << 20);
}

unsigned int I_format_hex_exception(I_format_type_exception x) {
    return x.opcode | (x.rd << 7) | (x.funct3 << 12) | (x.rs1 << 15) | (x.imm_5_0 << 20) | (x.imm_11_6 << 26);
}

unsigned int S_format_hex(S_format_type x) {
    return x.opcode | (x.imm_4_0 << 7) | (x.funct3 << 12) | (x.rs1 << 15) | (x.rs2 << 20) | (x.imm_11_5 << 25);
}

unsigned int B_format_hex(B_format_type x) {
    return x.opcode | (x.imm_11 << 7) |(x.imm_4_1 << 8) | (x.funct3 << 12) | (x.rs1 << 15) | (x.rs2 << 20) | (x.imm_10_5 << 25) | (x.imm_12 << 31);
}

unsigned int J_format_hex(J_format_type x) {
    return x.opcode | (x.rd << 7) | (x.imm_19_12 << 12) | (x.imm_11 << 20) | (x.imm_10_1 << 21) | (x.imm_20 << 31);
}

unsigned long long int U_format_hex(U_format_type x) {
    return x.opcode | (x.rd << 7) | (x.imm_31_12 << 12) ;
    
}
//register equa\ivalent number finder
// int register_finder(char *str) {
//     if (str == NULL || *str == '\0') {
//         return -1;
//     }
//     const char* registers_format1[] = {"x0","x1","x2","x3","x4","x5","x6","x7",
//                               "x8","x9","x10","x11","x12","x13","x14","x15",
//                               "x16","x17","x18","x19","x20","x21","x22","x23",
//                               "x24","x25","x26","x27","x28","x29","x30","x31"};
//     const char* registers_format2[] = {"zero","ra","sp","gp","tp","t0","t1","t2",
//                               "s0","s1","a0","a1","a2","a3","a4","a5",
//                               "a6","a7","s2","s3","s4","s5","s6","s7",
//                               "s8","s9","s10","s11","t3","t4","t5","t6"};
//     for(int i=0; i<=31; i++) {
//         if(strcmp(str, registers_format1[i]) == 0 || strcmp(str, registers_format2[i]) == 0) {
//             return i;
//         }
//     } 
//     return -1;
// } 
//helper function to assign register number to struct variables
void register_value_R_format(R_format_type *ins, char **str) {
    ins->rs2 = register_finder(str[3]);
    ins->rs1 = register_finder(str[2]);
    ins->rd = register_finder(str[1]);
}

void register_value_I_format1(I_format_type *ins, char **str) {
    ins->rs1 = register_finder(str[2]);
    ins->rd = register_finder(str[1]);
}

void register_value_I_format1_exception(I_format_type_exception *ins, char **str) {
    ins->rs1 = register_finder(str[2]);
    ins->rd = register_finder(str[1]);
}

void register_value_I_format2(I_format_type *ins, char **str) {
    ins->rs1 = register_finder(str[3]);
    ins->rd = register_finder(str[1]);
}

void register_value_S_format(S_format_type *ins, char **str) {
    ins->rs2 = register_finder(str[1]);
    ins->rs1 = register_finder(str[3]);
}

void register_value_B_format(B_format_type *ins, char **str) {
    ins->rs1 = register_finder(str[1]);
    ins->rs2 = register_finder(str[2]);
}

void register_value_J_format(J_format_type *ins, char **str) {
    ins->rd = register_finder(str[1]);
}

void register_value_U_format(U_format_type *ins, char **str) {
    ins->rd = register_finder(str[1]);
}
//helper function to assign immediate value to struct variable
void immediate_value_I_format1(I_format_type *ins, char **str){
    int imm;
    sscanf(str[3] , "%d" , &imm);
    ins->immediate = imm;
}

void immediate_value_I_format2(I_format_type *ins, char **str){
    int imm;
    sscanf(str[2] , "%d" , &imm);
    ins->immediate = imm;
}

void immediate_value_I_format1_exception(I_format_type_exception *ins, char **str){
    int imm;
    sscanf(str[3] , "%d" , &imm);
    ins->imm_5_0 = imm;
}

void immediate_value_S_format(S_format_type *ins, char **str){
    int imm;
    sscanf(str[2] , "%d" , &imm);
    int imm_part1=imm >> 5;
    ins->imm_11_5 = imm_part1;
    int imm_part2=imm-(imm_part1 << 5);
    ins->imm_4_0= imm_part2;
    ins->immediate= imm;
}

void immediate_value_B_format1(B_format_type *ins, char **str){
    int imm;
    sscanf(str[3] , "%d" , &imm);
    ins->immediate = imm;
    int x=imm >> 1;
    int x1=x >> 4;
    int x2=x - (x1 << 4);
    ins->imm_4_1= x2;
    int x3=x1 >> 6;
    int x4=x1 - (x3 << 6);
    ins->imm_10_5= x4;
    int x5=x >> 11;
    ins->imm_12= x5;
    int x6=x >> 10;
    int x7=x6 - (x5 << 1);
    ins->imm_11=x7;    
}

void immediate_value_B_format2(B_format_type *ins, int imm){
    ins->immediate=imm;
    int x=imm >> 1;
    int x1=x >> 4;
    int x2=x - (x1 << 4);
    ins->imm_4_1= x2;
    int x3=x1 >> 6;
    int x4=x1 - (x3 << 6);
    ins->imm_10_5= x4;
    int x5=x >> 11;
    ins->imm_12= x5;
    int x6=x >> 10;
    int x7=x6 - (x5 << 1);
    ins->imm_11=x7;  
}

void immediate_value_J_format1(J_format_type *ins, char **str){
    int imm;
    sscanf(str[2] , "%d" , &imm);
    ins->immediate = imm;   
    int x=imm >> 1;
    int x1=x >> 19;
    ins->imm_20=x1;
    int x2=x - (x1 << 19);
    int x3=x2 >> 11;
    ins->imm_19_12=x3;
    int x4=x2 - (x3 << 11);
    int x5=x4 >> 10;
    ins->imm_11=x5;
    int x6=x4 - (x5 << 10);
    ins->imm_10_1=x6; 
}

void immediate_value_J_format2(J_format_type *ins, int imm){
    ins->immediate=imm;
    int x=imm >> 1;
    int x1=x >> 19;
    ins->imm_20=x1;
    int x2=x - (x1 << 19);
    int x3=x2 >> 11;
    ins->imm_19_12=x3;
    int x4=x2 - (x3 << 11);
    int x5=x4 >> 10;
    ins->imm_11=x5;
    int x6=x4 - (x5 << 10);
    ins->imm_10_1=x6;  
}

void immediate_value_U_format(U_format_type *ins, char **str){
    unsigned long long int imm;
    char* endpointer;
    imm=strtoull(str[2] , &endpointer , 0);
    ins->immediate = imm;
    unsigned long long int x=imm << 12;
    unsigned long long int x1=x >> 12;
    ins->imm_31_12= x1;  
}
//function to split the string into further substrings
// char **string_split(char *string) {
//     static char *result[MAX_TOKENS + 1];
//     int a = 0;
//     char *substrings = strtok(string, " \t\n\r"); 
    
//     while (substrings != NULL && a < MAX_TOKENS) {
//         result[a++] = substrings;
//         substrings = strtok(NULL, " \t\n\r");
//     }
//     result[a] = NULL;
//     return result;
// }
// //function to check if the specified character is present in the string 
// int ischarinstring(char *string,char x){
//     for(int i=0;i<strlen(string);i++){
//         if(string[i]==x){
//             return 1;
//         }
//     }
//     return 0;
// }
// //function to check if there are non integer character in string
// int non_int_char_finder(char *str){
//     int len=strlen(str);
//     for(int i=0;i<len;i++){
//         if(!isdigit(str[i]) && str[i]!='-'){
//             return 1;
//         }
//     }
//     return 0;
// }
// //function to count number of token in dynamically allocated array
int countTokens(char **tokens) {
    int count = 0;
    while (tokens[count] != NULL) {
        count++;
    }
    return count;
}

void hex(FILE* input,MemEntry *mem_entries,uint64_t t_address) {
    //FILE *input, *output;
    char line[MAX_INPUT_SIZE];
    
    input = fopen("input.s", "r");
   if (input == NULL) {
        printf("Error opening input file");
        
    } 
    
    // output = fopen("output.hex", "w");
    // if (output == NULL) {
    //     printf("Error opening output file");
    //     fclose(input);
    //     return 0;
    // }
    int line_iterater=0;
    int label_position_iter=0;
    char *label_pointer[MAX_LINES];
    int array[MAX_LINES];
    //label handling
    while (fgets(line, sizeof(line), input) != NULL) {
        if (ischarinstring(line, ':')) {
            char *tokens_for_labels = strtok(line, ":\n\r");
            label_pointer[label_position_iter] = strdup(tokens_for_labels);
            array[label_position_iter] = line_iterater;
            label_position_iter++;
        }
        line_iterater++;
    }//error handling for multiple labels with same name
    for (int i=0;i<label_position_iter;i++){
        for (int j=i+1;j<label_position_iter;j++){
            if(strcmp(label_pointer[i],label_pointer[j])==0){
                printf("Error:Multiple labels with same name at Line %d and Line %d\n",array[i]+1,array[j]+1);
                break;
            }
        }
    }
    rewind(input);
    int line_iterater2=0;
    //instruction reading start here
    while (fgets(line, sizeof(line), input) != NULL) {
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
        if(ischarinstring(line_copy1,'.')==1){
            continue;
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
        if (tokens[0] == NULL) continue;
        int substrings=countTokens(tokens);
        //using if else to read the first substring of line
        //R-format start here
        if (strcmp(tokens[0], "add") == 0) {
            R_format_type ins = create_r_format();
            ins.funct3 = 0x0;
            ins.funct7 = 0x00;
            // if(substrings!=4){
            //     printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
            //     fprintf(output,"\n");
            //     line_iterater2=line_iterater2+1;
            //     continue;
            // }
            register_value_R_format(&ins, tokens);
            // if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
            //     printf("Error in line %d:Invalid register name\n",line_iterater2+1);
            //     fprintf(output,"\n");
            //     line_iterater2=line_iterater2+1;
            //     continue;
            // }
            unsigned int hex_code = r_format_hex(ins);
            for(int k=0;k<4;k++){
                mem_entries[t_address+k].value=(hex_code>> (k*8)) & 0xff;
            }
            t_address=t_address+4;
            
        // } else if (strcmp(tokens[0], "sub") == 0) {
        //     R_format_type ins = create_r_format();
        //     ins.funct3 = 0x0;
        //     ins.funct7 = 0x20;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_R_format(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = r_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "and") == 0) {
        //     R_format_type ins = create_r_format();
        //     ins.funct3 = 0x7;
        //     ins.funct7 = 0x00;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_R_format(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = r_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "or") == 0) {
        //     R_format_type ins = create_r_format();
        //     ins.funct3 = 0x6;
        //     ins.funct7 = 0x00;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_R_format(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = r_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "xor") == 0) {
        //     R_format_type ins = create_r_format();
        //     ins.funct3 = 0x4;
        //     ins.funct7 = 0x00;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_R_format(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = r_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "sll") == 0) {
        //     R_format_type ins = create_r_format();
        //     ins.funct3 = 0x1;
        //     ins.funct7 = 0x00;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_R_format(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = r_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "srl") == 0) {
        //     R_format_type ins = create_r_format();
        //     ins.funct3 = 0x5;
        //     ins.funct7 = 0x00;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_R_format(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = r_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "sra") == 0) {
        //     R_format_type ins = create_r_format();
        //     ins.funct3 = 0x5;
        //     ins.funct7 = 0x20;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_R_format(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = r_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        //     //I-format starts  here
        // } else if (strcmp(tokens[0], "addi") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x0;
        //     ins.opcode = 0b0010011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format1(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format1(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "andi") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x7;
        //     ins.opcode = 0b0010011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format1(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format1(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "ori") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x6;
        //     ins.opcode = 0b0010011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format1(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format1(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "xori") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x4;
        //     ins.opcode = 0b0010011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format1(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format1(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "slli") == 0) {
        //     I_format_type_exception ins;
        //     ins.funct3 = 0x1;
        //     ins.opcode = 0b0010011;
        //     ins.imm_11_6=0x00;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format1_exception(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format1_exception(&ins, tokens);
        //     if(ins.imm_5_0 <= -1 || ins.imm_5_0 >= 64){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex_exception(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "srli") == 0) {
        //     I_format_type_exception ins;
        //     ins.funct3 = 0x5;
        //     ins.opcode = 0b0010011;
        //     ins.imm_11_6=0x00;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format1_exception(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format1_exception(&ins, tokens);
        //     if(ins.imm_5_0 <= -1 || ins.imm_5_0 >= 64){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex_exception(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "srai") == 0) {
        //     I_format_type_exception ins;
        //     ins.funct3 = 0x5;
        //     ins.opcode = 0b0010011;
        //     ins.imm_11_6=0x10;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format1_exception(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format1_exception(&ins, tokens);
        //     if(ins.imm_5_0 <= -1 || ins.imm_5_0 >= 64){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex_exception(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "ld") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x3;
        //     ins.opcode = 0b0000011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "lw") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x2;
        //     ins.opcode = 0b0000011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "lh") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x1;
        //     ins.opcode = 0b0000011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "lb") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x0;
        //     ins.opcode = 0b0000011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "lwu") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x6;
        //     ins.opcode = 0b0000011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "lhu") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x5;
        //     ins.opcode = 0b0000011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "lbu") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x4;
        //     ins.opcode = 0b0000011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        //     //S-format starts here
        // } else if (strcmp(tokens[0], "sd") == 0) {
        //     S_format_type ins;
        //     ins.funct3 = 0x3;
        //     ins.opcode = 0b0100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_S_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_S_format(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = S_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "sw") == 0) {
        //     S_format_type ins;
        //     ins.funct3 = 0x2;
        //     ins.opcode = 0b0100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_S_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_S_format(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = S_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "sh") == 0) {
        //     S_format_type ins;
        //     ins.funct3 = 0x1;
        //     ins.opcode = 0b0100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_S_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_S_format(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = S_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "sb") == 0) {
        //     S_format_type ins;
        //     ins.funct3 = 0x0;
        //     ins.opcode = 0b0100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_S_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_S_format(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = S_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        //     //jalr I-format starts here
        // } else if (strcmp(tokens[0], "jalr") == 0) {
        //     I_format_type ins;
        //     ins.funct3 = 0x0;
        //     ins.opcode = 0b1100111;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_I_format2(&ins, tokens);
        //     if(ins.rd==-1 || ins.rs1==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_I_format2(&ins, tokens);
        //     if(ins.immediate <= -2049 || ins.immediate >= 2048){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = I_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        //     //B-format starts here
        // } else if (strcmp(tokens[0], "beq") == 0) {
        //     B_format_type ins;
        //     ins.funct3 = 0x0;
        //     ins.opcode = 0b1100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_B_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     int index=-1;
        //     for(int k=0;k<label_position_iter;k++){
        //            if(strcmp(tokens[3],label_pointer[k])==0){
        //                 index=array[k];
        //                 break;
        //             }
        //     }
        //     if (index!=-1){
        //         int imm=((index)-(line_iterater2))*4;
        //         immediate_value_B_format2(&ins, imm);
        //     }  
        //     if(index == -1 && non_int_char_finder(tokens[3])==0 ){
        //         immediate_value_B_format1(&ins, tokens);
        //     } 
        //     if(index == -1 && non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:no such label exists\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(ins.immediate <= -4097 || ins.immediate >= 4096){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = B_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "bne") == 0) {
        //     B_format_type ins;
        //     ins.funct3 = 0x1;
        //     ins.opcode = 0b1100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_B_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     int index=-1;
        //     for(int k=0;k<label_position_iter;k++){
        //            if(strcmp(tokens[3],label_pointer[k])==0){
        //                 index=array[k];
        //                 break;
        //             }
        //     }
        //     if (index!=-1){
        //         int imm=((index)-(line_iterater2))*4;
        //         immediate_value_B_format2(&ins, imm);
        //     }  
        //     if(index == -1 && non_int_char_finder(tokens[3])==0 ){
        //         immediate_value_B_format1(&ins, tokens);
        //     } 
        //     if(index == -1 && non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:no such label exists\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(ins.immediate <= -4097 || ins.immediate >= 4096){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = B_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "blt") == 0) {
        //     B_format_type ins;
        //     ins.funct3 = 0x4;
        //     ins.opcode = 0b1100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_B_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     int index=-1;
        //     for(int k=0;k<label_position_iter;k++){
        //            if(strcmp(tokens[3],label_pointer[k])==0){
        //                 index=array[k];
        //                 break;
        //             }
        //     }
        //     if (index!=-1){
        //         int imm=((index)-(line_iterater2))*4;
        //         immediate_value_B_format2(&ins, imm);
        //     }  
        //     if(index == -1 && non_int_char_finder(tokens[3])==0 ){
        //         immediate_value_B_format1(&ins, tokens);
        //     } 
        //     if(index == -1 && non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:no such label exists\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(ins.immediate <= -4097 || ins.immediate >= 4096){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = B_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "bge") == 0) {
        //     B_format_type ins;
        //     ins.funct3 = 0x5;
        //     ins.opcode = 0b1100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_B_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     int index=-1;
        //     for(int k=0;k<label_position_iter;k++){
        //            if(strcmp(tokens[3],label_pointer[k])==0){
        //                 index=array[k];
        //                 break;
        //             }
        //     }
        //     if (index!=-1){
        //         int imm=((index)-(line_iterater2))*4;
        //         immediate_value_B_format2(&ins, imm);
        //     }  
        //     if(index == -1 && non_int_char_finder(tokens[3])==0 ){
        //         immediate_value_B_format1(&ins, tokens);
        //     } 
        //     if(index == -1 && non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:no such label exists\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(ins.immediate <= -4097 || ins.immediate >= 4096){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = B_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "bltu") == 0) {
        //     B_format_type ins;
        //     ins.funct3 = 0x6;
        //     ins.opcode = 0b1100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_B_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     int index=-1;
        //     for(int k=0;k<label_position_iter;k++){
        //            if(strcmp(tokens[3],label_pointer[k])==0){
        //                 index=array[k];
        //                 break;
        //             }
        //     }
        //     if (index!=-1){
        //         int imm=((index)-(line_iterater2))*4;
        //         immediate_value_B_format2(&ins, imm);
        //     }  
        //     if(index == -1 && non_int_char_finder(tokens[3])==0 ){
        //         immediate_value_B_format1(&ins, tokens);
        //     } 
        //     if(index == -1 && non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:no such label exists\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(ins.immediate <= -4097 || ins.immediate >= 4096){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = B_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else if (strcmp(tokens[0], "bgeu") == 0) {
        //     B_format_type ins;
        //     ins.funct3 = 0x7;
        //     ins.opcode = 0b1100011;
        //     if(substrings!=4){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_B_format(&ins, tokens);
        //     if(ins.rs1==-1 || ins.rs2==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     int index=-1;
        //     for(int k=0;k<label_position_iter;k++){
        //            if(strcmp(tokens[3],label_pointer[k])==0){
        //                 index=array[k];
        //                 break;
        //             }
        //     }
        //     if (index!=-1){
        //         int imm=((index)-(line_iterater2))*4;
        //         immediate_value_B_format2(&ins, imm);
        //     }  
        //     if(index == -1 && non_int_char_finder(tokens[3])==0 ){
        //         immediate_value_B_format1(&ins, tokens);
        //     } 
        //     if(index == -1 && non_int_char_finder(tokens[3])==1){
        //         printf("Error in line %d:no such label exists\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(ins.immediate <= -4097 || ins.immediate >= 4096){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = B_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        //     //J-format starts here
        // } else if (strcmp(tokens[0], "jal") == 0) {
        //     J_format_type ins;
        //     ins.opcode = 0b1101111;
        //     if(substrings!=3){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_J_format(&ins, tokens);
        //     if(ins.rd==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     int index=-1;
        //     for(int k=0;k<label_position_iter;k++){
        //            if(strcmp(tokens[2],label_pointer[k])==0){
        //                 index=array[k];
        //                 break;
        //             }    
        //     }
        //     if (index!=-1){
        //         int imm=((index)-(line_iterater2))*4;
        //         immediate_value_J_format2(&ins, imm);
        //     }  
        //     if(index == -1 && non_int_char_finder(tokens[2])==0 ){
        //         immediate_value_J_format1(&ins, tokens);
        //     } 
        //     if(index == -1 && non_int_char_finder(tokens[2])==1){
        //         printf("Error in line %d:no such label exists\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     if(ins.immediate <= -1048577 || ins.immediate >= 1048576){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = J_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        //     //U-format starts here
        // } else if (strcmp(tokens[0], "lui") == 0) {
        //     U_format_type ins;
        //     ins.opcode = 0b0110111;
        //     if(substrings!=3){
        //         printf("Error in line %d:Instruction syntax is incorrect\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     register_value_U_format(&ins, tokens);
        //     if(ins.rd==-1){
        //         printf("Error in line %d:Invalid register name\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     immediate_value_U_format(&ins, tokens);
        //     if(ins.immediate >= 4294967296){
        //         printf("Error in Line %d:Invalid immediate/offset values\n",line_iterater2+1);
        //         fprintf(output,"\n");
        //         line_iterater2=line_iterater2+1;
        //         continue;
        //     }
        //     unsigned int hex_code = U_format_hex(ins);
        //     fprintf(output, "%08X\n", hex_code);
        // } else {//error check if the first substring is not equal to any given instruction
        //     printf("Error in Line %d:No such instruction exists\n",line_iterater2+1);
        //     fprintf(output,"\n");
        //     line_iterater2=line_iterater2+1;
        //     continue;
        // }
        line_iterater2=line_iterater2+1;
    }
    }
    
    fclose(input);
    //fclose(output);
    
} 