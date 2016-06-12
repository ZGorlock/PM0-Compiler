//Zachary Gill
//Alex McKeel
//COP 3402 Spring 2016
//Section 1
//pMachine
//02-08-2016

#include "main.h"


//Main Function

/*
    The main function for the pMachine compiler.
    This is called when the program is run.

    Input:
        The compiler directives:
            -l : Prints out the lexeme list.

    Output:
        0 if sucessfully executed
        -1 if there was an error
*/
int main(int argc, const char* argv[])
{
    //Lexer
    //input:  input.txt
    //output: cleaninput.txt
    //        lexemelist.txt
    //        lexemetable.txt

    if (!scan(checkDirectives(argc, argv, "-l")))
        return -1;


    //Parser
    //input:  lexemelist.txt
    //output: symlist.txt

    if (!parse())
        return -1;


    //Code Generator
    //output: mcode.txt

    if (!generate(checkDirectives(argc, argv, "-a")))
        return -1;


    //P-Machine
    //input:  mcode.txt
    //output: stacktrace.txt

    if (!pMachine(checkDirectives(argc, argv, "-vm")))
        return -1;



    if (DEBUG) system("pause");
    return 0;
}


//Functions

/*
    Determines if a particular directive is used.

    Input:
        argc : The number of arguments.
        argv : The list of arguments.
        directive : The directive to look for.

    Output:
        1 if the directive is used.
        0 otherwise.
*/
int checkDirectives(int argc, const char* argv[], char* directive)
{
    int i;
    for (i = 0; i < argc; i++) {
        if (!strcmp(argv[i], directive))
            return 1;
    }

    return 0;
}