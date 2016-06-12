//Zachary Gill
//Alex McKeel
//COP 3402 Spring 2016
//Section 1
//pMachine
//04-10-2016

#include "parser.h"


//Variables

struct symbol symbolTable[MAX_SYMBOL_TABLE_SIZE]; //holds the list of symbols in the program
int offsetArray[MAX_LEXI_LEVELS]; //holds the offsets of each lexigraphical level

int procedureCount = 0; //holds the number of sub procedures
int currentProcedure = 0; //holds the current procedure

int bufferIndex = 0; //for the lexeme buffer that has all our tokens
char *tokenBuffer[MAX_LEXEME_NUM]; //an array of char pointers i.e. strings.

int symbolIndex = 0; //index counter for symbolTable
int globalLevel = 0; //gives current level.
char *tokenStr; //acts as our universal token
int token; //hold the token type
int universalCount = 0; //for printing the lexeme thing


//Functions

/*
    Parses the lexemes and checks for syntax errors.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int parse()
{
    if (DEBUG) printf("Parser:\n");

    if (DEBUG) printf("Initializing offsets...\n");
    setOffsets();

    if (DEBUG) printf("Making lexeme buffer...\n");
    if (!makeBuffer())
        return 0;

    if (DEBUG) printf("Parsing program...\n");
    program();

    if (DEBUG) printf("Producing symlist.txt...\n");
    if (!printSymlist())
        return 0;

    printf("No errors, program is syntactically correct.\n");

    if (DEBUG) printf("\n");

    return 1;
}



//Functions

/*
    Prints the symbol list to a file.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int printSymlist()
{

    FILE* symlist;
    if (!openSymlistFile(&symlist, "w"))
        return 0;

    fprintf(symlist, "%-8s%-8s%-8s%-8s%-8s\n", "Name", "Type", "Level", "Value", "Address");
    int test = 0;

    for (test = 0; test < symbolIndex; test++)
    {
        struct symbol temp = symbolTable[test];

        if (temp.kind == CONST)
            fprintf(symlist, "%-8s%-8s%-8d%-8d%-8s", temp.name, "const", temp.level, temp.val, "-");
        else if (temp.kind == VAR)
            fprintf(symlist, "%-8s%-8s%-8d%-8d%-8d", temp.name, "var", temp.level, temp.val, temp.addr);
        else
            fprintf(symlist, "%-8s%-8s%-8d%-8d%-8d", temp.name, "proc", temp.level, temp.val, temp.addr);
        fprintf(symlist, "\n");
    }

    if (!closeSymlistFile(&symlist))
        return 0;

    return 1;
}

/*
    Prints the symbol list to the console.
*/
void printList()
{
    if (DEBUG) printf("Symbol List:\n");

    int s;
    for (s = 0; s < universalCount; s++)
    {
        printf("%s ", tokenBuffer[s]);
    }
    printf("\n");
}

/*
    Initializes offsets array.
*/
void setOffsets()
{
    int x;
    for (x = 0; x < MAX_LEXI_LEVELS; x++)
    {
        offsetArray[x] = 4; //we are reserving stack space for activation record
    }
}

/*
    Reads the lexemelist tokens and puts them all into a char array buffer.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int makeBuffer(){

    FILE* lexemelist;
    
    if (!openLexemelistFile(&lexemelist, "r"))
        return 0;

    int count = 0; //just to keep track of how many tokens we have

    while (!feof(lexemelist))
    {
        tokenBuffer[count] = calloc(MAX_IDENTIFIER_LENGTH + 1, sizeof(char)); //each element of the tokenBuffer is a string. Thus we must calloc
        fscanf(lexemelist, "%s", tokenBuffer[count]);
        count++;

        if (DEBUG) printf("Reading lexeme: %s\n", tokenBuffer[count - 1]);
    }
    universalCount = count;

    if (!closeLexemelistFile(&lexemelist))
        return 0;

    return 1;
}

/*
    Returns the next token using the buffer array.

    Output:
        The token type retrieved.
*/
int getToken()
{
    tokenStr = tokenBuffer[bufferIndex];
    token = atoi(tokenStr);
    bufferIndex++;

    if (DEBUG) printf("Getting token: %s\n", tokenStr);

    return token;
}

/*
    Parses a program production.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int program()
{
    setOffsets();
    getToken();

    currentProcedure = getFreeSet();
    int varSpace = block();
    
    if (token != periodsym)
        throwError(9);

    generateVariableSpace(currentProcedure, varSpace + 4);
    generateHalt(currentProcedure);

    return 1;
}

/*
    Parses a block production.

    Output:
        The number of variables in the block.
*/
int block()
{
    int varSpace = 0;

    if (token == constsym) {
        do {
            getToken();
            if (token != identsym)
                throwError(4);
            getToken(); //the name of the const
            makeSymbol(tokenStr, CONST);
            getToken();
            if (token != eqlsym) {
                if (token == becomessym)
                    throwError(1);
                else
                    throwError(3);
            }
            getToken();
            if (token != numbersym) {
                throwError(2);
            }
            int val = getToken(); //this is the actual number value
            symbolTable[symbolIndex - 1].val = val;//save the actual number value into the constant's table entry. have to go back one since we just incremented
            getToken();

        } while (token == commasym);
        if (token != semicolonsym)
            throwError(5);
        getToken();
    }

    if (token == varsym) {
        do {
            getToken();
            if (token != identsym)
                throwError(4);
            getToken(); //the name of the var
            makeSymbol(tokenStr, VAR);
            getToken();

            varSpace++;

        } while (token == commasym);

        if (token != semicolonsym)
            throwError(5);
        getToken();
    }

    int oldProcedure = currentProcedure;

    while (token == procsym) {
        getToken();
        if (token != identsym)
            throwError(4);
        getToken(); //the name of the procedure
        makeSymbol(tokenStr, PROC);

        getToken();
        if (token != semicolonsym)
            throwError(5);
        getToken();
        globalLevel++;//within the block is a new level.
        int varSubSpace = block();
        globalLevel--;
        if (token != semicolonsym)
            throwError(6);
        getToken();

        generateVariableSpace(currentProcedure, varSubSpace + 4);
    }

    currentProcedure = oldProcedure;

    statement();

    return varSpace;
}

/*
    Parses a statement production.
*/
void statement()
{
    if (token == identsym) {
        getToken(); //the name of the identifier
        struct symbol sym = getSymbol(tokenStr, globalLevel);
        if (sym.val == INT_MIN)
        {
            throwError(11);
        }
        if (sym.kind != VAR)
        {
            throwError(12);
        }
        if (!isInScope(sym, globalLevel)) {
            throwError(29);
        }

        getToken();
        if (token != becomessym)
        {
            if (token == eqlsym) throwError(1);
            throwError(13);
        }
        
        getToken();
        expression();

        generateBecomes(currentProcedure, sym, globalLevel);

    }
    else if (token == callsym) {
        getToken();
        if (token != identsym)
            throwError(14);
        getToken(); //the name of the procedure identifier to call
        struct symbol sym = getSymbol(tokenStr, globalLevel);
        if (sym.val == INT_MIN) //ensure ident exists
        {
            throwError(11);//the symbol does not exist!
        }
        if (sym.kind != PROC)//3 means its a procedure. if not, error
        {
            throwError(15);
        }
        if (!isInScope(sym, globalLevel)) {
            throwError(29);
        }

        getToken();

        generateCall(currentProcedure, sym, globalLevel);

    }
    else if (token == beginsym) {
        getToken();
        statement();
        while (token == semicolonsym) {
            getToken();
            statement();
        }

        if (token != endsym)
        {
            //if (isStatement())
            //{
            //  throwError(10);
            //}
            throwError(19);
        }
        getToken();

        if (currentProcedure > 0) //no return for main procedure
            generateReturn(currentProcedure);
    }
    else if (token == ifsym) {
        getToken();
        condition();
        if (token != thensym)
            throwError(16);
        getToken();
        
        generateConditionalJump(currentProcedure);
        int j1 = getCodeLength(currentProcedure) - 1;

        statement();

        generateJump(currentProcedure);
        int j2 = getCodeLength(currentProcedure) - 1;

        changeM(currentProcedure, j1, getCodeLength(currentProcedure));

        if (token == elsesym) {
            getToken();
            statement();
        }

        changeM(currentProcedure, j2, getCodeLength(currentProcedure));

    }
    else if (token == whilesym) {
        getToken();

        int c = getCodeLength(currentProcedure);

        condition();
        if (token != dosym)
            throwError(18);
        getToken();

        generateConditionalJump(currentProcedure);
        int j1 = getCodeLength(currentProcedure) - 1;

        statement();

        generateJump(currentProcedure);
        int j2 = getCodeLength(currentProcedure) - 1;

        changeM(currentProcedure, j1, getCodeLength(currentProcedure));
        changeM(currentProcedure, j2, c);

    }
    else if (token == readsym)
    {
        getToken();
        if (token != identsym)
            throwError(27);
        getToken();//gets the actual identsym
        struct symbol ident = getSymbol(tokenStr, globalLevel);
        if (ident.val == INT_MIN) //ensure ident exists
        {
            throwError(11);//the symbol does not exist!
        }
        if (ident.kind != VAR)//2 means its a var. if not, error
        {
            throwError(27);
        }
        if (!isInScope(ident, globalLevel)) {
            throwError(29);
        }

        getToken();

        generateRead(currentProcedure, ident, globalLevel);

    }
    else if (token == writesym)
    {
        getToken();
        if (token != identsym)
            throwError(28);
        getToken();//gets the actual identsym
        struct symbol ident = getSymbol(tokenStr, globalLevel);
        if (ident.val == INT_MIN) //ensure ident exists
        {
            throwError(11);//the symbol does not exist!
        }
        if (!isInScope(ident, globalLevel)) {
            throwError(29);
        }

        getToken();

        generateWrite(currentProcedure, ident, globalLevel);

    }
}

/*
    Parses a condition production.
*/
void condition()
{
    if (token == oddsym) {
        getToken();
        expression();

        generateRelOp(currentProcedure, oddsym - 1);
    }
    else {
        expression();
        int op = relOp();
        getToken();
        expression();

        generateRelOp(currentProcedure, op);
    }
}

/*
    Parses a relOp production.

    Output:
        The relational operator token.
*/
int relOp()
{
    if (token != eqlsym && token != neqsym &&
        token != lessym && token != leqsym &&
        token != gtrsym && token != geqsym) {
        throwError(20);
    }

    return token;
}

/*
    Parses an expression production.
*/
void expression()
{
    int preop = 0;
    if (token == plussym || token == minussym) { //+ or -
        preop = token - plussym + 1; //plus = 1, minus = 2
        getToken();
    }
    term(preop);

    while (token == plussym || token == minussym) {
        int op = token; //4 if plus, 5 if minus

        getToken();
        term(0);

        generateCalculation(currentProcedure, op);
    }
}

/*
    Parses a term production.

    Input:
        preop : The prepositional operator.
            1 = plus symbol.
            2 = minus symbol.
*/
void term(int preop)
{
    int sign = factor();

    switch (preop) {
    case 1 : //plus
        generatePosification(currentProcedure);
        break;
    case 2 : //minus
        generateNegation(currentProcedure);
        break;
    }

    while (token == multsym || token == slashsym) {
        int op = token; //6 if mult, 7 if div

        getToken();
        factor();

        generateCalculation(currentProcedure, op);
    }
}

/*
    Parses a factor production.

    Output:
        The sign of the factor.
            0 if positive.
            1 if negative.
            -1 by default.
*/
int factor()
{
    int sign = -1;

    if (token == identsym) {
        getToken(); //this is the name of the identifier
        struct symbol factorSym = getSymbol(tokenStr, globalLevel);
        if (factorSym.val == INT_MIN)
        {
            throwError(11);
        }
        if (factorSym.kind == PROC) //proc
        {
            throwError(21);
        }
        if (!isInScope(factorSym, globalLevel)) {
            throwError(29);
        }

        if (factorSym.kind == VAR) //var
            generateLoad(currentProcedure, factorSym, globalLevel);
        else if (factorSym.kind == CONST) //const
            generateLiteral(currentProcedure, factorSym.val);

        sign = (factorSym.val < 0); //1 if negative, 0 if positive
        
        getToken();
    }
    else if (token == numbersym) {
        getToken(); //this is the value of the number
        if (token > 99999) //5 digits maximum
            throwError(25);

        generateLiteral(currentProcedure, token);
        sign = (token < 0);

        getToken();
    }
    else if (token == lparentsym) {
        getToken();
        expression();
        if (token != rparentsym)
            throwError(22);
        getToken();
    }
    else {
        throwError(23);
    }

    return sign;
}

/*
    Checks if the current token is the beginning of another statement.

    Output:
        1 if it is the start of a statement.
        0 otherwise.
*/
int isStatement()
{
    struct symbol temp = getSymbol(tokenStr, globalLevel);
    if (temp.val != -1)//if this token is indeed in the symbol table...
    {
        return 1;// then we know this is the start of a statement.
    }

    if (token == callsym || token == beginsym || token == ifsym || token == whilesym || token == readsym || token == writesym)
    {
        return 1;
    }

    return 0;//not a statement, return 0.
}

/*
    Determines if the symbol is in scope.

    Input:
        sym : The symbol.
        currentLevel : The current level of the program.

    Output:
        1 if it is in scope.
        0 otherwise.
*/
int isInScope(struct symbol sym, int currentLevel)
{
    if (sym.level <= currentLevel)
        return 1;
    
    return 0;
}

/*
    Returns the symbol with the given name from the symbol table.

    Input:
        ident : The identifier of the symbol.
        currentLevel : The currentLevel of the program.

    Output:
        The symbol, if it exists, null otherwise.
        The symbol returned cannot be editted, it is only for comparison.
*/
struct symbol getSymbol(char* ident, int currentLevel)
{
    struct symbol temp;
    temp.val = INT_MIN;

    int i;
    for (i = 0; i < symbolIndex; i++) {
        if (!strcmp(ident, symbolTable[i].name)) {
            if (temp.val == INT_MIN) {
                temp = symbolTable[i];
            }
            else {
                if (symbolTable[i].level <= currentLevel && symbolTable[i].level - globalLevel > temp.level - globalLevel)
                    temp = symbolTable[i];
            }
        }
    }

    return temp;//just return junk
}

/*
    Adds a symbol to the symbol table.

    Input:
        token : The identifier of the symbol.
        type : The kind of the symbol.
*/
void makeSymbol(char* token, int type)
{
    //char *temp=calloc(MAX_IDENTIFIER_LENGTH + 1, sizeof(char));
    struct symbol symbolChunk;
    symbolChunk.kind = type;//either 1,2, or 3
    strcpy(symbolChunk.name, token);

    if (type == 1)//if this is a constant
    {

        symbolChunk.val = atoi(tokenBuffer[bufferIndex]);//get the actual const value. bufferIndex would already be pointing to the NEXT token, not the current one,
        //hence why i am using tokenBuffer[bufferIndex], not tokenBuffer[bufferIndex+1];

    }
    else if (type == 2)// if this is a var
    {
        symbolChunk.val = 0;
        symbolChunk.addr = offsetArray[globalLevel];
        offsetArray[globalLevel]++;//increment the offset inside by one, for any future variables
    }
    else if (type == 3)
    {
        symbolChunk.val = 0;
        procedureCount++;
        symbolChunk.addr = procedureCount;
        currentProcedure = getFreeSet();

    }

    symbolChunk.level = globalLevel;


    symbolTable[symbolIndex] = symbolChunk;
    symbolIndex++;

}

/*
    Throws a syntax error.

    Input:
        errorType : The type of the error.
*/
void throwError(int errorType)
{
    printError(errorType);

    if (DEBUG) system("pause");
    exit(errorType);
}

/*
    Prints out an error message for an error type.

    Input:
        errorType : The type of the error.
*/
void printError(int errorType)
{
    if (DEBUG) printList();

    printf("Error %d: ", errorType);

    switch (errorType) {
    case 1:
        printf("Use = instead of :=.\n");
        break;
    case 2:
        printf("= must be followed by a number.\n");
        break;
    case 3:
        printf("Identifier must be followed by = .\n");
        break;
    case 4:
        printf("const, var, procedure must be followed by identifier.\n");
        break;
    case 5:
        printf("Semicolon or comma missing.\n");
        break;
    case 6:
        printf("Incorrect symbol after procedure declaration.\n");
        break;
    case 7:
        printf("Statement expected.\n");
        break;
    case 8:
        printf("Incorrect symbol after statement part in block.\n");
        break;
    case 9:
        printf("Period expected.\n");
        break;
    case 10:
        printf("Semicolon between statements missing.\n");
        break;
    case 11:
        printf("Undeclared identifier.\n");
        break;
    case 12:
        printf("Assignment to constant or procedure is not allowed.\n");
        break;
    case 13:
        printf("Assignment operator expected.\n");
        break;
    case 14:
        printf("call must be followed by an identifier.\n");
        break;
    case 15:
        printf("Call of a constant or variable is meaningless.\n");
        break;
    case 16:
        printf("then expected.\n");
        break;
    case 17:
        printf("Semicolon or } expected.\n");
        break;
    case 18:
        printf("do expected.\n");
        break;
    case 19:
        printf("Incorrect symbol following statement.\n");
        break;
    case 20:
        printf("Relational operator expected.\n");
        break;
    case 21:
        printf("Expression must not contain a procedure identifier.\n");
        break;
    case 22:
        printf("Right parenthesis missing.\n");
        break;
    case 23:
        printf("The preceding factor cannot begin with this symbol.\n");
        break;
    case 24:
        printf("An expression cannot begin with this symbol.\n");
        break;
    case 25:
        printf("This number is too large.\n");
        break;
    case 26:
        printf("end expected.\n");
        break;
    case 27:
        printf("read must be followed by an variable identifier.\n");
        break;
    case 28:
        printf("write must be followed by an identifier.\n");
        break;
    case 29:
        printf("Symbol out of scope.\n");
        break;
    }
}
