//Zachary Gill
//Alex McKeel
//Sayeed Tahseen
//COP 3402 Spring 2016
//Section 1
//pMachine
//03-18-2016

#include "lexer.h"


//Variables

char token; //the current token
char lookahead; //the next token

FILE *input; //pointer to the input file
FILE *cleaninput; //pointer to the cleaninput file
FILE *lexemetable; //pointer to the lexemetable file
FILE *lexemelist; //pointer to the lexemelist file

struct lexeme lexemes[MAX_LEXEME_NUM]; //holds a list of lexemes that have been read
int lexemeCount = 0; //stores the number of lexemes in the lexemes array


//Functions

/*
    Scans the input and standardizes it.

    Input:
        lexemeDirective:
            1 if the compiler director detected the -l directive
            0 otherwise.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int scan(int lexemeDirective)
{
    if (DEBUG) printf("Lexer:\n");

    if (DEBUG) printf("Reading input.txt...\n");
    if (DEBUG) printf("Producing cleaninput.txt...\n");
    if (!readInput())
        return 0;

    if (DEBUG) printf("Reading cleaninput.txt...\n");
    if (!readCleanInput(lexemeDirective))
        return 0;

    printf("Successfully scanned input pl/0 code.\n");

    if (DEBUG) printf("\n");

    return 1;
}

/*
    Reads input.txt and produces cleaninput.txt.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int readInput()
{
    if (!openInputFile(&input, "r"))
        return 0;

    if (!openCleaninputFile(&cleaninput, "w"))
        return 0;

    char buffer[1024];
    int bufferIndex = 0;

    if (DEBUG) printf("Reading tokens:\n");

    do {
        token = fgetc(input);

        if (DEBUG) printf("%d ", token);

        if (token == 10) {
            if (DEBUG)
                fprintf(cleaninput, "\n");
            bufferIndex = 0;
        }

        if (isSpace(token)) { //put space characters into buffer, focus on meaningful chars
            buffer[bufferIndex++] = token;
        }
        else {
            if (token == '/') { //possible beginning of comment
                if ((lookahead = fgetc(input)) == '*') { //comment confirmed
                    removingComment();
                    bufferIndex = 0;
                }
                else {
                    ungetc(lookahead, input);
                }
            }

            if (isPrint(token) || isSpace(token) && token != '\0') {
                buffer[bufferIndex++] = token;
                int i;
                for (i = 0; i < bufferIndex; i++) {//print buffer to file
                    fputc(buffer[i], cleaninput);
                }
                bufferIndex = 0;
            }
        }

    } while (!feof(input));

    if (DEBUG) printf("\n");

    if (!closeCleaninputFile(&cleaninput))
        return 0;

    if (!closeInputFile(&input))
        return 0;

    return 1;
}

/*
    Used by readInput(). Reads to the end of a comment.
*/
void removingComment()
{
    if (!feof(input))
        token = fgetc(input);

    if (!feof(input))
        lookahead = fgetc(input);

    if (token == '*' && lookahead == '/') { //reached end of comment
        token = '\0';
        lookahead = '\0';
        return;
    }

    ungetc(lookahead, input);
    removingComment(); //keep looking
}

/*
    Reads cleaninput.txt and lexically parses the file.

    Input:
        lexemeDirective:
            1 if the compiler director detected the -l directive
            0 otherwise.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int readCleanInput(int lexemeDirective)
{
    if (!openCleaninputFile(&cleaninput, "r"))
        return 0;

    if (!lexemeDecoder()) //breaks file into lexemes
        return 0;

    if (!printLexemeList(lexemeDirective)) //create lexemelist.txt
        return 0;

    if (!printLexemeTable()) //create lexemetable.txt
        return 0;

    if (!closeCleaninputFile(&cleaninput))
        return 0;

    return 1;
}

/*
    Decodes the lexemes from tokens read from cleaninput.txt.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int lexemeDecoder()
{
    char* chunk = "";
    int length = 0;

    while (strlen(chunk = getNextToken()) > 0) { //keep getting tokens until file is empty

        if (DEBUG) printf("Decoding: %s\n", chunk);

        while (strlen(chunk) > 0) { //keep looping until token is eaten up

            if (isAlpha(chunk[0])) { //Alpha character
                if ((length = lexemeWordDecoder(chunk)) == -1)
                    return 0;

                chunk = eatUsedChars(chunk, length);
            }

            else if (isDigit(chunk[0])) { //Number character
                if ((length = lexemeNumberDecoder(chunk)) == -1)
                    return 0;

                chunk = eatUsedChars(chunk, length);
            }

            else { //Symbol character
                if ((length = lexemeSymbolDecoder(chunk)) == -1)
                    return 0;

                chunk = eatUsedChars(chunk, length);
            }

        }
    }

    return 1;
}

/*
    Decodes reserve word or identifier lexemes.

    Input:
        chunk : A string of characters that presumably contains one or more lexemes.

    Output:
        The length of the decoded lexeme.
        -1 if there was an error.
*/
int lexemeWordDecoder(char* chunk)
{
    struct lexeme l;
    l.token = calloc(MAX_IDENTIFIER_LENGTH + 1, sizeof(char));
    l.type = nulsym;

    int counter;
    for (counter = 0; counter < (int)strlen(chunk); counter++) { //loop through this chunk
        char currentChar = chunk[counter];
        if (!isAlpha(currentChar) && !isDigit(currentChar) && !(currentChar == '_')) {
            break; //once we hit something thats not a number or digit, stop.
        }
    }

    if (counter > MAX_IDENTIFIER_LENGTH) { //Per the professors rules, identifiers can't be more than 11 chars long.
        printf("Error: Identifiers cannot have more than 11 characters!\n");
        return -1;
    }

    int i;
    for (i = 0; i < counter; i++) { //copies over the lexeme to a new char array, so we can save it to a lexeme struct
        l.token[i] = chunk[i];
    }

    //recall strcmp returns 0 if the strings are equal. Hence we use ! to invert it to 1 if they match.
    if (!strcmp(l.token, "const")) {
        l.type = (int)constsym;
    }
    else if (!strcmp(l.token, "var")) {
        l.type = (int)varsym;
    }
    else if (!strcmp(l.token, "procedure")) {
        l.type = (int)procsym;
    }
    else if (!strcmp(l.token, "call")) {
        l.type = (int)callsym;
    }
    else if (!strcmp(l.token, "begin")) {
        l.type = (int)beginsym;
    }
    else if (!strcmp(l.token, "end")) {
        l.type = (int)endsym;
    }
    else if (!strcmp(l.token, "if")) {
        l.type = (int)ifsym;
    }
    else if (!strcmp(l.token, "then")) {
        l.type = (int)thensym;
    }
    else if (!strcmp(l.token, "else")) {
        l.type = (int)elsesym;
    }
    else if (!strcmp(l.token, "while")) {
        l.type = (int)whilesym;
    }
    else if (!strcmp(l.token, "do")) {
        l.type = (int)dosym;
    }
    else if (!strcmp(l.token, "read")) {
        l.type = (int)readsym;
    }
    else if (!strcmp(l.token, "write")) {
        l.type = (int)writesym;
    }
    else if (!strcmp(l.token, "odd")) {
        l.type = (int)oddsym;
    }
    else { //at this point it has to be an identifier
        l.type = (int)identsym;
    }

    lexemes[lexemeCount++] = l; //put the lexeme into the global array

    return strlen(l.token);
}

/*
    Decodes number lexemes.

    Input:
        chunk : A string of characters that presumably contains one or more lexemes.

    Output:
        The length of the decoded lexeme.
        -1 if there was an error
*/
int lexemeNumberDecoder(char* chunk)
{
    struct lexeme l;
    l.token = calloc(MAX_NUMBER_LENGTH + 1, sizeof(char));
    l.type = nulsym;

    int counter;
    for (counter = 0; counter < (int)strlen(chunk); counter++){ //loop through this chunk
        char currentChar = chunk[counter];
        if (!isDigit(currentChar)) {
            if (isAlpha(currentChar)) { //a symbol must follow a number
                printf("Error: Identifiers cannot start with a number!\n");
                return -1;
            }
            break; //once we hit something thats not a number or digit, stop.
        }
    }

    if (counter > MAX_NUMBER_LENGTH) { //Per the professors rules, identifiers can't be more than 11 chars long.
        printf("Error: Numbers cannot have more than 5 digits!\n");
        return -1;
    }

    int i;
    for (i = 0; i < counter; i++) { //copies over the lexeme to a new char array, so we can save it to a lexeme struct
        l.token[i] = chunk[i];
    }

    l.type = (int)numbersym;

    lexemes[lexemeCount++] = l;

    return strlen(l.token);
}

/*
    Decodes symbol lexemes.

    Input:
        chunk : A string of characters that presumably contains one or more lexemes.

    Output:
        The length of the decoded lexeme.
        -1 if there was an error
*/
int lexemeSymbolDecoder(char* chunk)
{
    struct lexeme l;
    l.token = calloc(MAX_SYMBOL_LENGTH + 1, sizeof(char));
    l.type = nulsym;

    if (chunk[0] == '\0')
        return 1;

    //if the first character is a plus symbol
    if (chunk[0] == '+')
    {
        l.type = (int)plussym; //make lType a plussym
        l.token[0] = '+';
    }

    //if the first character is a minus symbol
    else if (chunk[0] == '-')
    {
        l.type = (int)minussym; //make lType a minussym
        l.token[0] = '-';
    }

    //if the first character is a multiplication symbol
    else if (chunk[0] == '*')
    {
        l.type = (int)multsym; //make lType a multsym
        l.token[0] = '*';
    }

    //if the first character is a division symbol
    else if (chunk[0] == '/')
    {
        l.type = (int)slashsym; //make lType a slashsym
        l.token[0] = '/';
    }

    //if the first character is a equals symbol
    else if (chunk[0] == '=')
    {
        l.type = (int)eqlsym; //make lType a eqlsym
        l.token[0] = '=';
    }

    //if the first character is less-than symbol
    else if (chunk[0] == '<')
    {
        l.type = (int)lessym; //make lType a lessym
        l.token[0] = '<';

        if (strlen(chunk) >= 2) {

            //if the second character is an equals symbol, then it is a less-than-or-equal-to symbol
            if (chunk[1] == '=')
            {
                l.type = (int)leqsym; //make lType a leqsym
                l.token[1] = '=';
            }

            //if the second character is an greater-than symbol, then it is a not-equal symbol
            else if (chunk[1] == '>') {
                l.type = (int)neqsym; //make lType a neqsym
                l.token[1] = '>';
            }
        }

    }

    //if the first character is greater-than symbol
    else if (chunk[0] == '>')
    {
        l.type = (int)gtrsym; //make lType a gtrsym
        l.token[0] = '>';

        //if the second character is an equals symbol, then it is a greater-than-or-equal-to symbol
        if (strlen(chunk) >= 2 && chunk[1] == '=')
        {
            l.type = (int)geqsym; //make lType a geqsym
            l.token[1] = '=';
        }
    }

    //if the first character is a left-parantheses symbol
    else if (chunk[0] == '(')
    {
        l.type = (int)lparentsym; //make lType a lparentsym
        l.token[0] = '(';
    }

    //if the first character is a right-parantheses symbol
    else if (chunk[0] == ')')
    {
        l.type = (int)rparentsym; //make lType a rparentsym
        l.token[0] = ')';
    }

    //if the first character is a comma symbol
    else if (chunk[0] == ',')
    {
        l.type = (int)commasym; //make lType a commasym
        l.token[0] = ',';
    }

    //if the first character is a semi-colon symbol
    else if (chunk[0] == ';')
    {
        l.type = (int)semicolonsym; //make lType a semicolonsym
        l.token[0] = ';';
    }

    //if the first character is a period symbol
    else if (chunk[0] == '.')
    {
        l.type = (int)periodsym; //make lType a periodsym
        l.token[0] = '.';
    }

    //if the first character is an colod symbol and the second character is an equals symbol
    else if (chunk[0] == ':')
    {
        if (strlen(chunk) >= 2 && chunk[1] == '=') {
            l.type = (int)becomessym; //make lType a becomessym
            l.token[0] = ':';
            l.token[1] = '=';
        }
    }

    //else if nothing fits then an error has occurred
    else {
        printf("Error: Invalid symbol!\n");
        return -1;
    }

    lexemes[lexemeCount++] = l;

    return strlen(l.token); //return length
}

/*
    Reads the next token in the cleaninput file.
    Only works if lexeme is delimited by space.

    Output:
        The next token.
*/
char* getNextToken()
{
    char* t = calloc(MAX_TOKEN_LENGTH + 1, sizeof(char));
    int tIndex = 0;

    if (feof(cleaninput))
        return t;

    do {
        token = fgetc(cleaninput);
        if (isSpace(token) || !isPrint(token)) { //remove space from the stream as well
            if (tIndex > 0)
                break;
        }
        else {
            t[tIndex++] = token;
        }

    } while (!feof(cleaninput));

    t[tIndex++] = '\0';

    return t;
}

/*
    Prints the lexeme list to lexemelist.txt.

    Input:
        lexemeDirective:
            1 if the compiler director detected the -l directive
            0 otherwise.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int printLexemeList(int lexemeDirective)
{
    if (!openLexemelistFile(&lexemelist, "w"))
        return 0;

    int i;
    for (i = 0; i < lexemeCount; i++) {
        fprintf(lexemelist, "%d", lexemes[i].type); //print each lexeme type to the file

        if (lexemes[i].type == 2 || lexemes[i].type == 3) //for variables and numbers
            fprintf(lexemelist, " %s", lexemes[i].token); //print actual value as well

        if (i != lexemeCount - 1) //space delimited
            fprintf(lexemelist, " ");

        if (lexemeDirective == 1) {
            printf("%d", lexemes[i].type); //print each lexeme type to the file

            if (lexemes[i].type == 2 || lexemes[i].type == 3) //for variables and numbers
                printf(" %s", lexemes[i].token); //print actual value as well

            if (i != lexemeCount - 1) //space delimited
                printf(" ");
        }

    }

    if (lexemeDirective == 1)
        printf("\n");

    if (!closeLexemelistFile(&lexemelist))
        return 0;

    return 1;
}

/*
    Prints the lexeme list to lexemetable.txt.

    Output:
        0 if there was an error.
        1 otherwise.
*/
int printLexemeTable()
{
    if (!openLexemetableFile(&lexemetable, "w"))
        return 0;

    fprintf(lexemetable, "%-12s%-3s\n", "lexeme", "token type"); //formatting
    int i;
    for (i = 0; i < lexemeCount; i++) {
        fprintf(lexemetable, "%-12s%-3d", lexemes[i].token, lexemes[i].type); //print each lexeme

        if (i != lexemeCount - 1)
            fprintf(lexemetable, "\n");
    }

    if (!closeLexemetableFile(&lexemetable))
        return 0;

    return 1;
}

/*
    Gets a subtring of a char array.

    Input:
        string : A char array to get the substring from.
        start : The index to start at.

    Output:
        The substring of the char array.
*/
char* eatUsedChars(char* string, int start)
{
    char* newString = calloc(strlen(string), sizeof(char));

    int i;
    for (i = start; i < (int)strlen(string); i++) {
        newString[i - start] = string[i];
    }

    return newString;
}

/*
    Determines is a char is an alphabetic character.

    Input:
        c : The char.

    Output:
        1 if it is an alphabetic character.
        0 if it is not an alphabetic character.
*/
int isAlpha(char c)
{
    if ((c >= 65 && c <= 90) ||
        (c >= 97 && c <= 122))
        return 1;
    return 0;
}

/*
    Determines is a char is an numerical character.

    Input:
        c : The char.

    Output:
        1 if it is a numerical character.
        0 if it is not a numerical character.
*/
int isDigit(char c)
{
    if (c >= 48 && c <= 57)
        return 1;
    return 0;
}

/*
    Determines is a char is an printable character.

    Input:
        c : The char.

    Output:
        1 if it is a printable character.
        0 if it is not a printable character.
*/
int isPrint(char c)
{
    if (c >= 33 && c <= 126)
        return 1;
    return 0;
}

/*
    Determines is a char is an space character.

    Input:
        c : The char.

    Output:
        1 if it is a space character.
        0 if it is not a space character.
*/
int isSpace(char c)
{
    if (c == 32 || c == 9 || c == 13 || c == 0)
        return 1;
    return 0;
}
