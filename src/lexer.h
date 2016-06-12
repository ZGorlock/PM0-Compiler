//Zachary Gill
//Alex McKeel
//Sayeed Tahseen
//COP 3402 Spring 2016
//Section 1
//pMachine
//03-18-2016

#ifndef LEXER_H
#define LEXER_H

#include "global.h"

#include "io.h"


//Enums

//Token Types
enum token {
    nulsym = 1,        // \0
    identsym = 2,      // a variable
    numbersym = 3,     // a number
    plussym = 4,       // "+"
    minussym = 5,      // "-"
    multsym = 6,       // "*"
    slashsym = 7,      // "/"
    oddsym = 8,        // "odd"
    eqlsym = 9,        // "="
    neqsym = 10,       // "<>"
    lessym = 11,       // "<"
    leqsym = 12,       // "<="
    gtrsym = 13,       // ">"
    geqsym = 14,       // ">="
    lparentsym = 15,   // "("
    rparentsym = 16,   // ")"
    commasym = 17,     // ","
    semicolonsym = 18, // ";"
    periodsym = 19,    // "."
    becomessym = 20,   // ":="
    beginsym = 21,     // "begin"
    endsym = 22,       // "end"
    ifsym = 23,        // "if"
    thensym = 24,      // "then"
    whilesym = 25,     // "while"
    dosym = 26,        // "do"
    callsym = 27,      // "call"
    constsym = 28,     // "const"
    varsym = 29,       // "var"
    procsym = 30,      // "procedure"
    writesym = 31,     // "write"
    readsym = 32,      // "read"
    elsesym = 33       // "else"
} token_type;


//Structs

//Lexeme Structure
struct lexeme {
    int type; //Lexeme type
    char* token; //The actual token
}lexeme;


//Function Prototypes

int scan(int lexemeDirective);

int readInput();
void removingComment();
int readCleanInput(int lexemeDirective);

int lexemeDecoder();
int lexemeWordDecoder(char*);
int lexemeNumberDecoder(char*);
int lexemeSymbolDecoder(char*);
char* getNextToken();

int printLexemeList(int lexemeDirective);
int printLexemeTable();
char* eatUsedChars(char*, int);

int isAlpha(char);
int isDigit(char);
int isPrint(char);
int isSpace(char);


#endif