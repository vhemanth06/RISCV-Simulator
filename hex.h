
#ifndef HEX_H
#define HEX_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#include "run.h"
#include <stdint.h> 
#define MAX_ADDRESS 0x50001
#define MAX_TOKENS 50
#define MAX_INPUT_SIZE 100 
#define MAX_LINES 500

//newly defined struct varaibles for every format type
typedef struct {
    int funct7;
    int rs2;
    int rs1;
    int funct3;
    int rd;
    int opcode;
} R_format_type;

R_format_type create_r_format() ;

typedef struct {
    int immediate;
    int rs1;
    int funct3;
    int rd;
    int opcode;
} I_format_type;

typedef struct {
    int imm_11_6;
    int imm_5_0;
    int rs1;
    int funct3;
    int rd;
    int opcode;
} I_format_type_exception;

typedef struct {
    int imm_11_5;
    int rs2;
    int rs1;
    int funct3;
    int imm_4_0;
    int opcode;
    int immediate;
} S_format_type;

typedef struct {
    int imm_12;
    int imm_10_5;
    int rs2;
    int rs1;
    int funct3;
    int imm_4_1;
    int imm_11;
    int opcode;
    int immediate;
} B_format_type;

typedef struct {
    int imm_20;
    int imm_10_1;
    int imm_11;
    int imm_19_12;
    int rd;
    int opcode;
    int immediate;
} J_format_type;

typedef struct {
    unsigned long long int imm_31_12;
    unsigned long long int rd;
    unsigned long long int opcode;
    unsigned long long int immediate;
} U_format_type;
//Machine code caculation functions
unsigned int r_format_hex(R_format_type x);

unsigned int I_format_hex(I_format_type x) ;

unsigned int I_format_hex_exception(I_format_type_exception x) ;

unsigned int S_format_hex(S_format_type x) ;

unsigned int B_format_hex(B_format_type x) ;

unsigned int J_format_hex(J_format_type x) ;

unsigned long long int U_format_hex(U_format_type x);
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
void register_value_R_format(R_format_type *ins, char **str) ;

void register_value_I_format1(I_format_type *ins, char **str) ;

void register_value_I_format1_exception(I_format_type_exception *ins, char **str) ;

void register_value_I_format2(I_format_type *ins, char **str) ;

void register_value_S_format(S_format_type *ins, char **str) ;

void register_value_B_format(B_format_type *ins, char **str) ;
void register_value_J_format(J_format_type *ins, char **str);

void register_value_U_format(U_format_type *ins, char **str);
//helper function to assign immediate value to struct variable
void immediate_value_I_format1(I_format_type *ins, char **str);

void immediate_value_I_format2(I_format_type *ins, char **str);

void immediate_value_I_format1_exception(I_format_type_exception *ins, char **str);

void immediate_value_S_format(S_format_type *ins, char **str);

void immediate_value_B_format1(B_format_type *ins, char **str);

void immediate_value_B_format2(B_format_type *ins, int imm);

void immediate_value_J_format1(J_format_type *ins, char **str);

void immediate_value_J_format2(J_format_type *ins, int imm);

void immediate_value_U_format(U_format_type *ins, char **str);
int countTokens(char **tokens) ;

void hex(FILE* input,MemEntry *mem_entries,uint64_t t_address);
#endif
