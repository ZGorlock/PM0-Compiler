//Zachary Gill
//COP 3402 Spring 2016
//Section 1
//pMachine
//04-28-2016

#ifndef IO_H
#define IO_H

#include "global.h"


//Constants

static const char* INPUT_FILE = "input.txt";
static const char* CLEANINPUT_FILE = "cleaninput.txt";
static const char* LEXEMETABLE_FILE = "lexemetable.txt";
static const char* LEXEMELIST_FILE = "lexemelist.txt";
static const char* SYMBOLLIST_FILE = "symlist.txt";
static const char* STACKTRACE_FILE = "stacktrace.txt";
static const char* MCODE_FILE = "mcode.txt";


//Function Prototypes

int openInputFile(FILE** input, const char* type);
int closeInputFile(FILE** input);

int openCleaninputFile(FILE** cleaninput, const char* type);
int closeCleaninputFile(FILE** cleaninput);

int openLexemelistFile(FILE** lexemelist, const char* type);
int closeLexemelistFile(FILE** lexemelist);

int openLexemetableFile(FILE** lexemetable, const char* type);
int closeLexemetableFile(FILE** lexemetable);

int openSymlistFile(FILE** symlist, const char* type);
int closeSymlistFile(FILE** symlist);

int openMcodeFile(FILE** mcode, const char* type);
int closeMcodeFile(FILE** mcode);

int openStacktraceFile(FILE** stacktrace, const char* type);
int closeStacktraceFile(FILE** stacktrace);


#endif