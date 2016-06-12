//Zachary Gill
//COP 3402 Spring 2016
//Section 1
//pMachine
//02-08-2016

#include "stack.h"


//Variables

int stack[MAX_STACK_HEIGHT + 1]; //holds the stack of the running program
int ars[MAX_LEXI_LEVELS]; //stores the location of activation records in the stack
int arCount = 0; //stores the number of current activation records


//Functions

/*
    Sets the value of a frame in the stack.

    Input:
        index : The index of the stack frame to write.
        frame : The information to store on the stack.
*/
void putFrame(int index, int frame)
{
    if (index > MAX_STACK_HEIGHT)
        printf("Error: Stack overflow!\n");

    stack[index] = frame;
}

/*
    Retrieves a frame from the stack.

    Input:
        index : The index of the stack frame to read.

    Output:
        The value of the frame that was read.
*/
int getFrame(int index)
{
    if (index > MAX_STACK_HEIGHT)
        printf("Error: Attempting to access a stack index that doesn't exist!\n");

    return stack[index];
}

/*
    Prints the contents of the stack to the stacktrace output file.

    Input:
        stacktrace : The file pointer to the stacktrace file.
        r : The current program registers
*/
void printStack(FILE* stacktrace, struct registers r, int stacktraceDirective)
{
    int ar = 0;
    int i;
    for (i = 1; i <= r.SP; i++) {
        if (i == ars[ar]) {
            fprintf(stacktrace, " |");

            if (stacktraceDirective == 1) {
                printf(" |");
            }

            ar++;
        }

        fprintf(stacktrace, " %d", stack[i]);

        if (stacktraceDirective == 1) {
            printf(" %d", stack[i]);
        }
    }
}

/*
    Finds the base in the stack some levels down.

    Input:
        l : The L value in the instruction format
        base : The base we are looking for.

    Output:
        The location in the stack of the base.
*/
int base(int l, int base) // l stand for L in the instruction format
{
    int b1; //find base L levels down
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}