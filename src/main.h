//Zachary Gill
//Alex McKeel
//COP 3402 Spring 2016
//Section 1
//pMachine
//04-10-2016

#ifndef MAIN_H
#define MAIN_H

#include "global.h"
#include "pMachine.h"
#include "stack.h"
#include "instruction.h"
#include "io.h"
#include "lexer.h"
#include "parser.h"
#include "generator.h"


//Function Prototypes

int main(int argc, const char* argv[]);

int checkDirectives(int argc, const char* argv[], char* directive);


#endif