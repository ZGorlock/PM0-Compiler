//Zachary Gill
//Alex McKeel
//COP 3402 Spring 2016
//Section 1
//pMachine
//04-28-2016

#include "generator.h"


//Variables

struct instruction finalCode[MAX_CODE_SETS * MAX_CODE_LENGTH]; //holds the finalized code
int finalInstructionCount = 0;

struct instruction generatedCode[MAX_CODE_SETS][MAX_CODE_LENGTH]; //holds the compiled mcode sets
int instructionCount[MAX_CODE_SETS]; //stores the number of instructions in the code
int inUse[MAX_CODE_SETS]; //stores whether a set is in use or not.

FILE* mcode; //file pointer to the mcode file


//Funcitons

/*
    Produces the mcode file.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int generate(int mcodeDirective)
{
    if (DEBUG) printf("Code Generator:\n");

    if (DEBUG) printf("Preparing code chunks...\n");
    prepareMcode();

    if (DEBUG) printf("Producing mcode.txt...\n");
    if (!openMcodeFile(&mcode, "w"))
        return 0;

    printMcode(mcodeDirective);

    if (!closeMcodeFile(&mcode))
        return 0;

    printf("Successfully compiled pl/0 code.\n");

    if (DEBUG) printf("\n");

    return 1;
}

/*
    Combines code sets into the final mcode.
*/
void prepareMcode()
{
    int sets = 0;
    int i;
    for (i = 0; i < MAX_CODE_SETS; i++) {
        if (instructionCount[i] == 0) {
            sets = i;
            break;
        }
    }

    if (sets == 0)
        return;

    generateProcedureJumps(FINAL_CODE, sets);

    for (i = sets - 1; i >= 0; i--) {
        changeM(FINAL_CODE, i, finalInstructionCount); //update jump to accurate address

        int j;
        for (j = 0; j < instructionCount[i]; j++) {
            struct instruction inst = generatedCode[i][j];
            appendCode(FINAL_CODE, inst.OP, inst.L, inst.M);

            if (inst.OP == OP_JMP || inst.OP == OP_JPC) //increase jumps by procedure offset
                changeM(FINAL_CODE, finalInstructionCount - 1, inst.M + finalCode[i].M + 1);
        }
    }
}

/*
    Prints the mcode instructions to the mcode file.
*/
void printMcode(int mcodeDirective)
{
    int i;
    for (i = 0; i < finalInstructionCount; i++) {
        if (DEBUG) printf("Producing Instruction: %d %d %d\n", finalCode[i].OP, finalCode[i].L, finalCode[i].M);

        putInstruction(finalCode[i]);

        if (mcodeDirective == 1) {
            printf("%d %d %d\n", finalCode[i].OP, finalCode[i].L, finalCode[i].M);
        }
    }


}

/*
    Prints an instruction to the mcode file.

    Input:
        instruction : The instruction to print.
*/
void putInstruction(struct instruction instruction)
{
    fprintf(mcode, "%d %d %d\n", instruction.OP, instruction.L, instruction.M);
}

/*
    Adds an instruction to the end of the instruction array.

    Input:
        set : The code set, -1 for final code set.
        op : The op-code of the instruction to add.
        l : The lexigraphical level of the instruction to add.
        m : The data element of the instruction to add.
*/
void appendCode(int set, int op, int l, int m)
{
    if (set == FINAL_CODE) {
        finalCode[finalInstructionCount] = newInstruction(op, l, m);
        finalInstructionCount++;
    }
    else {
        generatedCode[set][instructionCount[set]] = newInstruction(op, l, m);
        instructionCount[set]++;
    }
}

/*
    Adds an instruction at the beginning of the instruction array.
    
    Input:
        set : The code set, -1 for final code set.
        op : The op-code of the instruction to add.
        l : The lexigraphical level of the instruction to add.
        m : The data element of the instruction to add.
*/
void prependCode(int set, int op, int l, int m)
{
    insertCode(set, op, l, m, 0);
}

/*
    Adds an instruction at a certain index of the instruction array.
    
    Input:
        set : The code set, -1 for final code set.
        op : The op-code of the instruction to add.
        l : The lexigraphical level of the instruction to add.
        m : The data element of the instruction to add.
        index : The index of where to insert the instruction.
*/
void insertCode(int set, int op, int l, int m, int index)
{
    if (set == FINAL_CODE) {
        if (index < 0 || index > finalInstructionCount - 1) {
            if (finalInstructionCount == 0)
                appendCode(set, op, l, m);
            return;
        }
    }
    else {
        if (index < 0 || index > instructionCount[set] - 1) {
            if (instructionCount[set] == 0)
                appendCode(set, op, l, m);
            return;
        }
    }

    makeSpaceInCode(set, 1, index);

    if (set == FINAL_CODE) {
        finalCode[index] = newInstruction(op, l, m);
        finalInstructionCount++;
    }
    else {
        generatedCode[set][index] = newInstruction(op, l, m);
        instructionCount[set]++;
    }
}

/*
    Makes space for new instructions in the instruction array.

    Input:
        set : The code set, -1 for final code set.
        size : The size of the space to make.
        index : The starting index of the space.
*/
void makeSpaceInCode(int set, int size, int index)
{
    if (size <= 0)
        return;

    if (set == FINAL_CODE) {
        if (index < 0 || index > finalInstructionCount - 1)
            return;
    }
    else {
        if (index < 0 || index > instructionCount[set] - 1)
            return;
    }

    int i;
    if (set == FINAL_CODE) {
        for (i = finalInstructionCount - 1; i >= index; i--)
            finalCode[i + size] = finalCode[i];
    }
    else {
        for (i = instructionCount[set] - 1; i >= index; i--)
            generatedCode[set][i + size] = generatedCode[set][i];
    }
}

/*
    Produces code for a numerical literal.

    Input:
        set : The code set, -1 for final code set.
        value : The value of the literal.
*/
void generateLiteral(int set, int value)
{
    appendCode(set, OP_LIT, 0, value);
}

/*
    Produces code for loading a symbol identifier.

    Input:
        set : The code set, -1 for final code set.
        ident : The symbol identifier.
        currentLevel : The current level of the program.
*/
void generateLoad(int set, struct symbol ident, int currentLevel)
{
    appendCode(set, OP_LOD, currentLevel - ident.level, ident.addr);
}

/*
    Produces code for storing a symbol identifier value.

    Input:
        set : The code set, -1 for final code set.
        ident : The symbol identifier.
        currentLevel : The current level of the program.
*/
void generateBecomes(int set, struct symbol ident, int currentLevel)
{
    appendCode(set, OP_STO, currentLevel - ident.level, ident.addr);
}

/*
    Produces code for a calculation.

    Input:
        set : The code set, -1 for final code set.
        token : The calculation token.
*/
void generateCalculation(int set, int token)
{
    appendCode(set, OP_OPR, 0, token - (plussym - OPR_ADD));
}

/*
    Produces code for a relational operator.

    Input:
        set : The code set, -1 for final code set.
        token : The relational operator token.
*/
void generateRelOp(int set, int token)
{
    appendCode(set, OP_OPR, 0, token - (eqlsym - OPR_EQL));
}

/*
    Produces code for a negation.

    Input:
        set : The code set, -1 for final code set.
*/
void generateNegation(int set)
{
    appendCode(set, OP_OPR, 0, OPR_NEG);
}

/*
    Produces code for a posification.

    Input:
        set : The code set, -1 for final code set.
*/
void generatePosification(int set)
{
    generateLiteral(set, generatedCode[set][getCodeLength(set) - 1].M);
    generateLiteral(set, 0);
    generateCalculation(set, OPR_LSS + (plussym - OPR_ADD));

    generateConditionalJump(set);
    int j1 = getCodeLength(set) - 1;

    generateCalculation(set, OPR_NEG + (plussym - OPR_ADD));

    changeM(set, j1, getCodeLength(set));
}

/*
    Produces code for creating variable space.

    Input:
        set : The code set, -1 for final code set.
        space : The number of variables and constants in the block.
*/
void generateVariableSpace(int set, int space)
{
    prependCode(set, OP_INC, 0, space);
}

/*
    Produces code for preliminary procedure jumps.

    Input:
        set : The code set, -1 for final code set.
        space : The number of procedures to add jumps for.
*/
void generateProcedureJumps(int set, int space)
{
    int i;
    for (i = 0; i < space; i++)
        generateJump(set);
}

/*
    Produces code for a call.

    Input:
        set : The code set, -1 for final code set.
        ident : The procedure identifier.
        currentLevel : The current level of the program.
*/
void generateCall(int set, struct symbol ident, int currentLevel)
{
    appendCode(set, OP_CAL, currentLevel - ident.level, ident.addr);
}

/*
    Produces code for a IO read.

    Input:
        set : The code set, -1 for final code set.
        ident : The variable identifier.
        currentLevel : The currentLevel of the program.
*/
void generateRead(int set, struct symbol ident, int currentLevel)
{
    appendCode(set, OP_SIO2, 0, 2);
    appendCode(set, OP_STO, currentLevel - ident.level, ident.addr);
}

/*
    Produces code for an IO write.

    Input:
        set : The code set, -1 for final code set.
        ident : The identfier.
        currentLevel : The current level of the program.
*/
void generateWrite(int set, struct symbol ident, int currentLevel)
{
    appendCode(set, OP_LOD, currentLevel - ident.level, ident.addr);
    appendCode(set, OP_SIO1, 0, 1);
}

/*
    Produces code for a generic jump.

    Input:
        set : The code set, -1 for final code set.
*/
void generateJump(int set)
{
    appendCode(set, OP_JMP, 0, 0);
}

/*
    Produces code for a generic conditional jump.

    Input:
        set : The code set, -1 for final code set.
*/
void generateConditionalJump(int set)
{
    appendCode(set, OP_JPC, 0, 0);
}

/*
    Produces code for a return.

    Input:
        set : The code set, -1 for final code set.
*/
void generateReturn(int set)
{
    appendCode(set, OP_OPR, 0, OPR_RET);
}

/*
    Produces code for a halt.

    Input:
        set : The code set, -1 for final code set.
*/
void generateHalt(int set)
{
    appendCode(set, OP_SIO3, 0, 3);
}

/*
    Returns the code length of a code set.

    Input:
        set : The code set, -1 for final code set.

    Output:
        The length of the code set.
*/
int getCodeLength(int set)
{
    if (set == FINAL_CODE)
        return finalInstructionCount;
    else
        return instructionCount[set];
}

/*
    Edits the M value of an instruction.

    Input:
        set : The code set, -1 for final code set.
        index : The index in the code set.
        m : The new m value.
*/
void changeM(int set, int index, int m)
{
    if (set == FINAL_CODE) {
        if (index < 0 || index > finalInstructionCount - 1)
            return;
        finalCode[index].M = m;
    }
    else {
        if (index < 0 || index > instructionCount[set] - 1)
            return;
        generatedCode[set][index].M = m;
    }
}

/*
    Returns the next free code set index.

    Output:
        The next free code set index, -1 if there are no free sets.
*/
int getFreeSet()
{
    int i;
    for (i = 0; i < MAX_CODE_SETS; i++) {
        if (inUse[i] == 0) {
            inUse[i] = 1;
            return i;
        }
    }

    return -1;
}


void tmp()
{
    //PROGRAM
    //block period
    //
    //jump past declarations to actual code
    //down there
    //  inc 0 (variableCount + 4)
    //have jumps for each procedures at top
    //
    //for halts
    //this happens at the "end." of the main procedure
    //SIO3 0 3



    //BLOCK

    //constdeclaration vardeclaration proceduredeclaration statement
    //
    //procedure a;
    //
    //const c1 = 1, c2 = 2;
    //var v1, v2, v3, v4;
    //
    //procedure b;
    //...
    //
    //begin
    //end.
    //
    //
    //jump a
    //jump b
    //b {
    //}
    //a {
    //}

    //CONSTDECLARATION
    //[const ident eql num {comma ident eql num} semicolon]
    //
    //lit 0 value
    //store it to variable address youre declaring it to

    //VARDECLARATION
    //[var ident {comma ident} semicolon]
    //
    //lit 0 0
    //store to variable address youre declaring it to

    //PROCEDURE DECLARATION
    //{procedure ident semicolon block semicolon}
    //recursive, taken care of automatically probably
    //keep track of level



    //STATEMENTS
    //[one of the following]

    //CALL
    //call ident
    //
    //cal level (proc #)
    //this will jump to top and hit a jump that will go to the procedures begin

    //BEGIN
    //begin statement {semicolon statement} end

    //IF
    //if condition then statement [else statement]
    //
    //0 condition
    //1 jpc 4
    //2 statement
    //3 jpc 4
    //4
    //
    //if condition then statement else statement
    //0 condition
    //1 jpc 4
    //2 statement //if
    //3 jpc 6
    //4 statement //else
    //5 jpc 6
    //6

    //WHILE
    //while condition do statement
    //
    //0 condition
    //1 jpc 4
    //2 statement //do
    //3 jmp 0
    //4 

    //READ
    //read ident
    //
    //SIO2 0 2
    //sto what is being read

    //WRITE
    //write ident
    //
    //lod what is being written
    //SIO1 0 1



    //CONDITION
    //odd expression | expression relop expression
    //
    //for odd
    //  load expression
    //  opr 0 odd
    //
    //for expressions
    //  lod e1
    //  lod e2
    //  opr 0 x
    //
    //n > 0
    //  lod n    //further down on stack than 0
    //  lod 0
    //  opr 0 gtr
    //
    //jpc jumps if it is 0 so it will jump if the condition is false
    //jumps to after what happens if the condition is true
    //jumps will be taken care of in other sections 



    //EXPRESSION
    //[plus | minus] term {(+ | -) term}
    //term -> factor {(* | /) term}
    //factor -> ident | num | lparen expression rparen
    //
    //a = b + c * 3 - 8
    //ident a eql ident b plus ident c times lit 3 minus lit 8
    //            --F1---
    //            --T1---
    //                    ------F2-----
    //                                 -----F3-----
    //                    ------------T2-----------
    //                                             -----F4----
    //                                             -----T3----
    //            --------------------E1----------------------
    //all terms are on the same precedence
    //
    //make t1
    //  lod f1
    //make t2
    //  lod f2
    //  lof f3
    //  times
    //plus
    //make t3
    //  lod f4
    //minus
    //sto a
    //
    //expression
    //
    //with parentheses it recursively makes an expression, automatically taken care of probably
    //
    //if theres a plus or minus at the beginning of an expression
    //for minus, opr 0 neg but this has to go after the variable is loaded
    //for plus, same thing but test if it is negative and only neg is it is

}