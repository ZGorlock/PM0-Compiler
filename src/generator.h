//Zachary Gill
//Alex McKeel
//COP 3402 Spring 2016
//Section 1
//pMachine
//04-28-2016

#ifndef GENERATOR_H
#define GENERATOR_H

#include "global.h"

#include "lexer.h"
#include "instruction.h"
#include "io.h"


//Constants

#define FINAL_CODE -1


//Enums

//OpCodes
enum {
    OP_LIT = 1,
    OP_OPR = 2,
    OP_LOD = 3,
    OP_STO = 4,
    OP_CAL = 5,
    OP_INC = 6,
    OP_JMP = 7,
    OP_JPC = 8,
    OP_SIO1 = 9,
    OP_SIO2 = 10,
    OP_SIO3 = 11
}OpCode;

//OprCodes
enum {
    OPR_RET = 0,
    OPR_NEG = 1,
    OPR_ADD = 2,
    OPR_SUB = 3,
    OPR_MUL = 4,
    OPR_DIV = 5,
    OPR_ODD = 6,
    OPR_MOD = 7,
    OPR_EQL = 8,
    OPR_NEQ = 9,
    OPR_LSS = 10,
    OPR_LEQ = 11,
    OPR_GTR = 12,
    OPR_GEQ = 13
}OprCode;


//Function Prototypes

int generate(int mcodeDirective);

void prepareMcode();
void printMcode(int mcodeDirective);
void putInstruction(struct instruction instruction);

void appendCode(int set, int op, int l, int m);
void prependCode(int set, int op, int l, int m);
void insertCode(int set, int op, int l, int m, int index);
void makeSpaceInCode(int set, int size, int index);

void generateLiteral(int set, int value);
void generateLoad(int set, struct symbol ident, int currentLevel);
void generateBecomes(int set, struct symbol ident, int currentLevel);
void generateCalculation(int set, int token);
void generateRelOp(int set, int token);
void generateNegation(int set);
void generatePosification(int set);
void generateVariableSpace(int set, int space);
void generateProcedureJumps(int set, int space);
void generateCall(int set, struct symbol ident, int currentLevel);
void generateRead(int set, struct symbol ident, int currentLevel);
void generateWrite(int set, struct symbol ident, int currentLevel);
void generateJump(int set);
void generateConditionalJump(int set);
void generateReturn(int set);
void generateHalt(int set);

int getCodeLength(int set);
void changeM(int set, int index, int m);
int getFreeSet();


#endif