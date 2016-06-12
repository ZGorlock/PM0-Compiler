//Zachary Gill
//COP 3402 Spring 2016
//Section 1
//pMachine
//02-08-2016

#ifndef PMACHINE_H
#define PMACHINE_H

#include "global.h"

#include "instruction.h"
#include "stack.h"
#include "io.h"


//Function Prototypes

int pMachine(int stacktraceDirective);

void run(int stacktraceDirective);
void fetch();
void execute(int stacktraceDirective);
void logFetch();
void logExecute(int stacktraceDirective);
void initializeRegisters();

int readMcode();
int read();
void initializeLog();


#endif