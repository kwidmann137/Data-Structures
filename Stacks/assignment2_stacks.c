/**********************************************************************
CS2123 assignment2_stacks.c by Kyle Widmann
Purpose:
    This program takes a file supplied by user and reads each line, which
    is expected to be an int.  A stack is implemented using dynamic memory.
    If the int is -99999 the stack is popped.  Otherwise the number is
    pushed onto the stack.  The stack increased by chunks of 10, but never
    decreases.
Command Parameters:
    assignment2 -i datafile.txt
Input:
    numbers supplied by datafile.txt from Command Line

    -The file is assumed to be nothing but integers.
    -All integers >-99999 will be pushed onto stack
    -Reading -99999 will cause the stack to pop
Results:
    Whenever the stack is popped the program prints elements remaning
    after the pop, and what integer was popped.  The program also prints
    whenver the stack size grows, as well as what the old and new stack
    sizes are.
Returns:
    0 - normal
Notes:

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Typedef for our stack
typedef struct stack{
    int iCount;
    int iStackSize;
    int iInfo[];
}STACK;

//Define our definitions for True, False and Pop
#define TRUE 1
#define FALSE 0
#define POP -99999

//Declaration of functions
STACK *makeStack();
STACK *increaseStack(STACK *pStack);
int stackPush(int x, STACK **ppStack);
int stackPop(STACK **ppStack);
int stackEmpty(STACK *pStack);
int stackFull(STACK *pStack);
void getFile(int argc, char *argv[], char **ppszInputFile);
void readFile();

/**************************** MAIN FUNCTION ************************/
int main(int argc, char *argv[])
{
    printf("Assignment 2 written by Kyle Widmann\n\n");

    //Pointer to input file from CLI, and stack to be used across program
    char *pszInputFile = NULL;
    STACK *pStack = makeStack();

    //Get the filename from the CLI
    getFile(argc, argv, &pszInputFile);

    //Open and read the file, pushing/popping as necessary
    readFile(pszInputFile, &pStack);

    //free pStack
    free(pStack);

    return 0;
}

/********************************* readFile ******************************
oid readFile(char *pszInputFile, STACK **ppStack)
Purpose: Read through the input file, and based on input, push the int to
    the stack or pop the stack

Parameters:
    I   char *pszInputFile          pointer to the string for input file name
    I/O STACK **ppStack             double pointer to the stack created in main
Notes:
    Input is assumed to be a valid int
**************************************************************************/

void readFile(char *pszInputFile, STACK **ppStack){
    char szInputBuffer[100];    //Input buffer to read from file
    int iInput;                 //variable to store the int read from file
    int iPop=0;                 //variable to store the popped int
    FILE *pFile;                //pointer to input file

    //Open the file
    pFile = fopen(pszInputFile, "r");
    if(pFile == NULL)
        printf("Error opening file;");

    // printf("pFile addres: %p\n", pFile);

    while(fgets(szInputBuffer, 100, pFile) != NULL){
        if(sscanf(szInputBuffer, "%d", &iInput) == 1){
            // printf("input: %d... \t", iInput);
            if(iInput == POP){
                // printf("about to Pop\n");
                iPop = stackPop(ppStack);
                if(iPop == FALSE){
                    printf("There was an error popping the stack\n");
                }
                else{
                    printf("Number of elements after popping: %d \t\t Integer popped: %d\n", (*ppStack)->iCount, iPop);
                }
            }
            else{
                stackPush(iInput, ppStack);
            }
        }
    }

    if(pFile != NULL){
        fclose(pFile);
        pFile = NULL;
    }
}

/********************************* makeStack ******************************
STACK *makeStack()
Purpose:  Dynamically allocate memory for a new stack that can store 10 int
    Returns a pointer to the address of the stack created.

Parameters:
    N/A
Notes:
**************************************************************************/

STACK *makeStack(){
    STACK *stack = (STACK *)malloc(sizeof(STACK)+sizeof(int)*10);
    // printf("size of stack: %lu\n", sizeof(*stack));
    if(stack == NULL){
        printf("Error allocating stack\n");
        return NULL;
    }
    stack->iInfo[0] = 0;
    stack->iCount = 0;
    stack->iStackSize = 10;
    return stack;
}

/********************************* increaseStack ******************************
STACK *increaseStack(STACK *pStack)
Purpose:Take the current stack pointer, realloc the stack to add 10 more spots
    for ints, and return the new address of the stack

Parameters:
    I   STACK *pStack           pointer to the old stack

Notes:
**************************************************************************/

STACK *increaseStack(STACK *pStack){
    pStack = (STACK *)realloc(pStack, (sizeof(STACK) + sizeof(int)*(pStack->iStackSize + 10)));
    if(pStack == NULL){
        printf("Error allocating stack\n");
        return NULL;
    }
    pStack->iStackSize += 10;
    //Print out that the stack has grown.
    printf("Stack capacity has grown from %d to %d\n", pStack->iStackSize - 10, pStack->iStackSize);
    return pStack;
}

/********************************* stackPush ******************************
int stackPush(int x, STACK **ppStack)
Purpose: Add the provided int to the top of the stack.

Parameters:
    I   int x               the integer that was read from the input file
    I/O STACK **ppStack     double pointer to the stack

Notes:
    return True if successfull, otherwise False
**************************************************************************/

int stackPush(int x, STACK **ppStack){
    if(stackFull(*ppStack) == TRUE){
        //increase stack size
        *ppStack = increaseStack(*ppStack);
    }

    //set first element
    if(stackEmpty(*ppStack) == TRUE){
        (*ppStack)->iInfo[(*ppStack)->iCount] = x;
        (*ppStack)->iCount = (*ppStack)->iCount + 1;
    }
    else
    {
        //Set the next element in stack
        (*ppStack)->iInfo[(*ppStack)->iCount] = x;
        (*ppStack)->iCount = (*ppStack)->iCount +1;
    }

    //if stack was not successfully updated
    if((*ppStack)->iInfo[(*ppStack)->iCount] != x)
        return FALSE;

    return TRUE;
}

/********************************* stackPop ******************************
int stackPop(STACK **ppStack)
Purpose: return the top integer from the stack, if stack is empty return
    False.

Parameters:
    I/O     STACK **ppStack     double pointer to stack we are popping
Notes:
**************************************************************************/

int stackPop(STACK **ppStack){
    int iPop;

    // if the stack is not empty
    if(stackEmpty(*ppStack) == FALSE){
        iPop = (*ppStack)->iInfo[(*ppStack)->iCount-1];
        (*ppStack)->iCount -= 1;
        return iPop;
    }

    return FALSE;
}

/********************************* stackEmpty ******************************
int stackEmpty(STACK *pStack)
Purpose:Return True is stack is empty, False otherwise

Parameters:
    I   STACK *pStack       Pointer to stack we are analyzing
Notes:
**************************************************************************/

int stackEmpty(STACK *pStack){
    if(pStack->iCount > 0){
        return FALSE;
    }
    return TRUE;
}

/********************************* stackFull ******************************
int stackFull(STACK *pStack)
Purpose:
    Return True is stack is full, False otherwise

Parameters:
    I   STACK *pStack       Pointer to stack we are analyzing
Notes:
**************************************************************************/

int stackFull(STACK *pStack){
    if(pStack->iCount == pStack->iStackSize){
        return TRUE;
    }
    return FALSE;
}

/********************************* getFile ******************************
void getFile(int argc, char *argv[], char **ppszInputFile)
Purpose:
    Reads the CLI to assign file name containing student info to pszInputFile
Parameters:
    I   int argc                    the count of command line arguments
    I   char argv[]                 Array of the command line arguments
    I/O char **ppszInputFile        Double pointer to character string for the file name
Notes:
**************************************************************************/

void getFile(int argc, char *argv[], char **ppszInputFile)
{
    int i;

    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            printf("Error: Switch expected");
        // determine which switch it is
        switch (argv[i][1])
        {
        case 'i':                   // Book File Name
            if (++i >= argc)
                printf("Error: Missing Switch\n");
            // check for too long of a file anme
            else
                *ppszInputFile = argv[i];
            break;
        default:
            printf("Error: Switch expected");
        }
    }
}