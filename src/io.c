//Zachary Gill
//COP 3402 Spring 2016
//Section 1
//pMachine
//04-28-2016

#include "io.h"


//Funcitons

/*
    Opens the input.txt file stream.

    Input:
        input : The input file pointer.
        type : The type of file i/o.
            "r" : Read
            "w" : Write

    Output:
        0 if there was an error.
        1 otherwise.
*/
int openInputFile(FILE** input, const char* type)
{
    *input = fopen(INPUT_FILE, type);
    if (*input == NULL) {
        printf("Error: Unable to open input file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Closes the input.txt file stream.

    Input:
        input : The input file pointer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int closeInputFile(FILE** input)
{
    if (fclose(*input) != 0) {
        printf("Error: Unable to close input file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Opens the cleaninput.txt file stream.

    Input:
        cleaninput : The cleaninput file pointer.
        type : The type of file i/o.
            "r" : Read
            "w" : Write

    Output:
        0 if there was an error.
        1 otherwise.
*/
int openCleaninputFile(FILE** cleaninput, const char* type)
{
    *cleaninput = fopen(CLEANINPUT_FILE, type);
    if (*cleaninput == NULL) {
        printf("Error: Unable to open cleaninput file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Closes the cleaninput.txt file stream.

    Input:
        cleaninput : The cleaninput file pointer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int closeCleaninputFile(FILE** cleaninput)
{
    if (fclose(*cleaninput) != 0) {
        printf("Error: Unable to close cleaninput file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Opens the lexemelist.txt file stream.

    Input:
        lexemelist : The lexemelist file pointer.
        type : The type of file i/o.
            "r" : Read
            "w" : Write

    Output:
        0 if there was an error.
        1 otherwise.
*/
int openLexemelistFile(FILE** lexemelist, const char* type)
{
    *lexemelist = fopen(LEXEMELIST_FILE, type);
    if (*lexemelist == NULL) {
        printf("Error: Unable to open lexemelist file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Closes the lexemelist.txt file stream.

    Input:
        lexemelist : The lexemelist file pointer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int closeLexemelistFile(FILE** lexemelist)
{
    if (fclose(*lexemelist) != 0) {
        printf("Error: Unable to close lexemelist file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Opens the lexemetable.txt file stream.

    Input:
        lexemetable : The lexemetable file pointer.
        type : The type of file i/o.
            "r" : Read
            "w" : Write

    Output:
        0 if there was an error.
        1 otherwise.
*/
int openLexemetableFile(FILE** lexemetable, const char* type)
{
    *lexemetable = fopen(LEXEMETABLE_FILE, type);
    if (*lexemetable == NULL) {
        printf("Error: Unable to open lexemetable file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Closes the lexemetable.txt file stream.

    Input:
        lexemetable : The lexemetable file pointer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int closeLexemetableFile(FILE** lexemetable)
{
    if (fclose(*lexemetable) != 0) {
        printf("Error: Unable to close lexemetable file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Opens the symlist.txt file stream.

    Input:
        symlist : The symlist file pointer.
        type : The type of file i/o.
            "r" : Read
            "w" : Write

    Output:
        0 if there was an error.
        1 otherwise.
*/
int openSymlistFile(FILE** symlist, const char* type)
{
    *symlist = fopen(SYMBOLLIST_FILE, type);
    if (symlist == NULL) {
        printf("Error: Unable to open symbollist file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Closes the symlist.txt file stream.

    Input:
        symlist : The symlist file pointer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int closeSymlistFile(FILE** symlist)
{
    if (fclose(*symlist) != 0) {
        printf("Error: Unable to close symbollist output file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Opens the mcode.txt file stream.

    Input:
        mcode : The mcode file pointer.
        type : The type of file i/o.
            "r" : Read
            "w" : Write

    Output:
        0 if there was an error.
        1 otherwise.
*/
int openMcodeFile(FILE** mcode, const char* type)
{
    *mcode = fopen(MCODE_FILE, type);
    if (*mcode == NULL) {
        printf("Error: Unable to open mcode file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Closes the mcode.txt file stream.

    Input:
        mcode : The mcode file pointer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int closeMcodeFile(FILE** mcode)
{
    if (fclose(*mcode) != 0) {
        printf("Error: Unable to close mcode file stream!");
        return 0;
    }
    return 1;
}

/*
    Opens the stacktrace.txt file stream.

    Input:
        stacktrace : The stacktrace file pointer.
        type : The type of file i/o.
            "r" : Read
            "w" : Write

    Output:
        0 if there was an error.
        1 otherwise.
*/
int openStacktraceFile(FILE** stacktrace, const char* type)
{
    *stacktrace = fopen(STACKTRACE_FILE, type);
    if (*stacktrace == NULL) {
        printf("Error: Unable to open stacktrace file stream!\n");
        return 0;
    }
    return 1;
}

/*
    Closes the stacktrace.txt file stream.

    Input:
        stacktrace : The stacktrace file pointer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int closeStacktraceFile(FILE** stacktrace)
{
    if (fclose(*stacktrace) != 0) {
        printf("Error: Unable to close stacktrace file stream!");
        return 0;
    }
    return 1;
}