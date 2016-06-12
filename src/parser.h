//Zachary Gill
//Alex McKeel
//COP 3402 Spring 2016
//Section 1
//pMachine
//04-10-2016

#ifndef PARSER_H
#define PARSER_H

#include "global.h"

#include "lexer.h"
#include "generator.h"
#include "io.h"


//Function Prototypes

int parse();

int printSymlist();
void printList();

void setOffets();
int makeBuffer();
int getToken();
void setOffsets();

int program();
int block();
void constDeclaration();
void varDevlaration();
void procedureDeclaration();
void statement();
void condition();
int relOp();
void expression();
void term(int preop);
int factor();
void number();
void ident();
void digit();
void letter();

int isStatement();
int isInScope(struct symbol sym, int currentLevel);
struct symbol getSymbol(char* ident, int currentLevel);
void makeSymbol(char* token, int type);

void throwError(int errorType);
void printError(int errorType);


#endif