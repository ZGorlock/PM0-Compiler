//Zachary Gill
//COP 3402 Spring 2016
//Section 1
//pMachine
//02-08-2016

#include "pMachine.h"


//Variables

struct registers r; //holds the current registers of the running program
FILE* stacktrace; //pointer to the stacktrace file


//Functions

/*
    Initializes and runs the pMachine.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int pMachine(int stacktraceDirective)
{
    if (DEBUG) printf("P-Machine:\n");

    if (DEBUG) printf("Opening stacktrace.txt...\n");
    if (!openStacktraceFile(&stacktrace, "w"))
        return 0;

    if (DEBUG) printf("Reading mcode.txt...\n");
    if (!readMcode()) //read instructions from file
        return 0;

    if (DEBUG) printf("Running pMachine...\n");
    run(stacktraceDirective); //run pMachine

    if (DEBUG) printf("Closing stacktrace.txt...\n");
    if (!closeStacktraceFile(&stacktrace))
        return 0;

    printf("Successfully executed pm/0 code.\n");

    if (DEBUG) printf("\n");

    return 1;
}

/*
    Runs the pMachine simulation.
*/
void run(int stacktraceDirective)
{
    initializeRegisters();
    initializeLog();

    do {
        fetch();
        logFetch();
        execute(stacktraceDirective);
        logExecute(stacktraceDirective);
    } while (!halt && r.PC != 0);
}

/*
    Fetches the next instruction from the instruction array.
*/
void fetch()
{
    r.IR = getInstruction(r.PC);
    r.PC++;
}

/*
    Executes the current instruction in the simulation.
*/
void execute(int stacktraceDirective)
{
    switch (r.IR.OP) {
    case 1: //LIT
        executeLIT();
        break;
    case 2: //OPR
        switch (r.IR.M) {
        case 0: //RET
            executeOPR_RET();
            break;
        case 1: //NEG
            executeOPR_NEG();
            break;
        case 2: //ADD
            executeOPR_ADD();
            break;
        case 3: //SUB
            executeOPR_SUB();
            break;
        case 4: //MUL
            executeOPR_MUL();
            break;
        case 5: //DIV
            executeOPR_DIV();
            break;
        case 6: //ODD
            executeOPR_ODD();
            break;
        case 7: //MOD
            executeOPR_MOD();
            break;
        case 8: //EQL
            executeOPR_EQL();
            break;
        case 9: //NEQ
            executeOPR_NEQ();
            break;
        case 10: //LSS
            executeOPR_LSS();
            break;
        case 11: //LEQ
            executeOPR_LEQ();
            break;
        case 12: //GTR
            executeOPR_GTR();
            break;
        case 13: //GEQ
            executeOPR_GEQ();
            break;
        }
        break;
    case 3: //LOD
        executeLOD();
        break;
    case 4: //STO
        executeSTO();
        break;
    case 5: //CAL
        executeCAL();
        break;
    case 6: //INC
        executeINC();
        break;
    case 7: //JMP
        executeJMP();
        break;
    case 8: //JPC
        executeJPC();
        break;
    case 9: //SIO
        executeSIO_1();
        break;
    case 10: //SIO
        executeSIO_2(read());
        break;
    case 11: //SIO
        executeSIO_3(stacktrace, stacktraceDirective);
        break;
    }
}

/*
    Prints information about the fetched instruction to the output file.
*/
void logFetch()
{
    fprintf(stacktrace, "%2d%7s   %-6d%-6d", r.PC - 1, opCodeName(r.IR.OP), r.IR.L, r.IR.M);
}

/*
    Prints information about the current register and stack state to the output file.
*/
void logExecute(int stacktraceDirective)
{
    if (r.IR.OP == 11) //dont print if halted
        return;

    fprintf(stacktrace, "%-6d%-6d%-6d", r.PC, r.BP, r.SP);
    printStack(stacktrace, r, 0);
    fprintf(stacktrace, "\n");

    if (stacktraceDirective == 1) {
        printf("%-6d%-6d%-6d", r.PC, r.BP, r.SP);
        printStack(stacktrace, r, stacktraceDirective);
        printf("\n");
    }
}

/*
    Initializes the registers to their default values at the start of the simulation.
*/
void initializeRegisters()
{
    r.SP = 0; //Stack Pointer
    r.BP = 1; //Base Pointer
    r.PC = 0; //Program Counter

    r.IR.OP = 0; //Instruction Register
    r.IR.L = 0;  //Instruction Register
    r.IR.M = 0;  //Instruction Register
}

/*
    Loads the instructions from the mcode.txt file.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int readMcode()
{
    FILE* mcode;
    if (!openMcodeFile(&mcode, "r"))
        return 0;

    int op = 0;
    int l = 0;
    int m = 0;

    while (!feof(mcode)) {
        fscanf(mcode, "%d %d %d", &op, &l, &m);
        fscanf(mcode, "\n");

        if (DEBUG) printf("Reading Instruction: %d %d %d\n", op, l, m);

        addInstruction(newInstruction(op, l, m));
    }

    if (!closeMcodeFile(&mcode))
        return 0;

    printInstructions(stacktrace);

    return 1;
}

/*
    Reads an int from the user for input.

    Output:
        The user input value.
*/
int read()
{
    int in = 0;
    printf("Input a value: ");
    scanf("%d", &in);
    return in;
}

/*
    Prints preliminary information to the second section of the output file.
*/
void initializeLog()
{
    fprintf(stacktrace, "%26s%6s%6s%10s\n", "pc", "bp", "sp", "stack");
    fprintf(stacktrace, "%-24s%-6d%-6d%-6d\n", "Initial values", r.PC, r.BP, r.SP);
}