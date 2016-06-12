//Zachary Gill
//COP 3402 Spring 2016
//Section 1
//pMachine
//02-08-2016

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "global.h"


//Function Prototypes

struct instruction newInstruction(int op, int l, int m);

struct instruction getInstruction(int index);
void addInstruction(struct instruction i);
void printInstructions(FILE* stacktrace);
const char* opCodeName(int op);
const char* oprName(int m);

void executeLIT();
void executeOPR_RET();
void executeOPR_NEG();
void executeOPR_ADD();
void executeOPR_SUB();
void executeOPR_MUL();
void executeOPR_DIV();
void executeOPR_ODD();
void executeOPR_MOD();
void executeOPR_EQL();
void executeOPR_NEQ();
void executeOPR_LSS();
void executeOPR_LEQ();
void executeOPR_GTR();
void executeOPR_GEQ();
void executeLOD();
void executeSTO();
void executeCAL();
void executeINC();
void executeJMP();
void executeJPC();
void executeSIO_1();
void executeSIO_2(int input);
void executeSIO_3(FILE* stacktrace, int stacktraceDirective);


#endif