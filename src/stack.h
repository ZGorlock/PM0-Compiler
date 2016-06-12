//Zachary Gill
//COP 3402 Spring 2016
//Section 1
//pMachine
//02-08-2016

#ifndef STACK_H
#define STACK_H

#include "global.h"


//Function Prototypes

void putFrame(int index, int frame);
int getFrame(int index);
void printStack(FILE* stacktrace, struct registers r, int stacktraceDirective);
int base(int l, int base);


#endif