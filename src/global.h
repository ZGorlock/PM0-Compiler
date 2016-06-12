//Zachary Gill
//Alex McKeel
//COP 3402 Spring 2016
//Section 1
//pMachine
//02-08-2016

#ifndef GLOBAL_H
#define GLOBAL_H

#define DEBUG 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


//Enums

//Constants
enum SizeConstants {
    MAX_STACK_HEIGHT = 2000,
    MAX_CODE_LENGTH = 500,
    MAX_CODE_SETS = 100,
    MAX_LEXI_LEVELS = 5,
    MAX_LEXEME_NUM = 10000,
    MAX_TOKEN_LENGTH = 1024,
    MAX_IDENTIFIER_LENGTH = 11,
    MAX_NUMBER_LENGTH = 5,
    MAX_SYMBOL_LENGTH = 2,
    MAX_SYMBOL_TABLE_SIZE = 100
}SizeConstants;

//Offsets
enum RegisterOffset {
    BP_OFFSET = 3,
    PC_OFFSET = 4
}RegisterOffset;

enum AROffset {
    FV_OFFSET = 1,
    SL_OFFSET = 2,
    DL_OFFSET = 3,
    RETA_OFFSET = 4
}AROffset;

enum SymbolKind {
    CONST = 1,
    VAR = 2,
    PROC = 3
};


//Structs

//Instruction Structure
struct instruction {
    int OP; //opcode
    int L;  //L
    int M;  //M
}instruction;

//Register Structure
struct registers {
    int SP; //Stack Pointer
    int BP; //Base Pointer
    int PC; //Program Counter
    struct instruction IR; //Instruction Register
}registers;

struct symbol
{
    int kind; //const = 1, var = 2, proc = 3
    char name[12]; //name
    int val; //number (ASCII value)
    int level; //L level
    int addr; //M address
} symbol;


//Variables

//pMachine
extern struct registers r; //stores the registers of the simulation

//stack
extern int stack[MAX_STACK_HEIGHT + 1]; //stores the stack for the running simulation
extern int ars[MAX_LEXI_LEVELS]; //stores the location of activation records in the stack
extern int arCount; //stores the number of current activation records

//instruction
extern struct instruction code[MAX_CODE_LENGTH]; //stores the code from mcode.txt
extern int codeSize; //stores the length of the code being executed
extern int halt; //flag to halt the execution of the simulation

//parser
extern int offsetArray[MAX_LEXI_LEVELS];//the purpose of this is to keep track of the stack pointer for each lexi level. each array index contains a number. this number is the topmost part of the stack for that AR
extern struct symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];

#endif