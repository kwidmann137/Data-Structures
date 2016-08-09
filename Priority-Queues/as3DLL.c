#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE{
    int iInfo;
    struct NODE* pPrev;
    struct NODE* pNext;
}NODE;

//Defines

//global variables
int iTotalChecks = 0;

//function declarations
void getFile(int argc, char *argv[], char **ppszInputFile);
void readFile(char *pszInputFile, NODE **ppHead, NODE **ppRear);
void printList(NODE *p);
NODE *makeNode(int x);
void insertNode(NODE **ppF, NODE **ppR, int x, int iCount);
NODE *removeNode(NODE *p);
void freeNodes(NODE *p);

int main(int argc, char *argv[])
{
    printf("Assignment 3 Problem 2 written by Kyle Widmann\n\n");

    //pointer to input file and head of linear linked list
    char *pszInputFile = NULL;
    NODE *pHead = NULL;
    NODE *pRear = NULL;

    //get file provided from CLI
    getFile(argc, argv, &pszInputFile);

    //read the file and process input
    readFile(pszInputFile, &pHead, &pRear);

    //print total number of checks
    printf("Total checks: %d\n", iTotalChecks);

    //free all nodes
    freeNodes(pHead);

    return 0;
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

/********************************* readFile ******************************
void readFile(char *pszInputFile, NODE **ppHead, NODE **ppRear)
Purpose: Reads the input file, line by line.  Input file format is
    <COMMAND> <int> or <COMMAND> where command is either insert followed by
    an int or remove.  Insert will add the following in to the linked list
    in ascending order, and remove will remove the lowest numbers from the
    list.

Parameters:
    I       char*pszInputFile       pointer to the name of the input file
    I/O     NODE **ppHead           double pointer to the head of the linked list
    I/O     NODE *ppRear            double pointer to the rear of the linked list

Notes:
**************************************************************************/
void readFile(char *pszInputFile, NODE **ppHead, NODE **ppRear){
    char szInputBuffer[100];
    FILE *pFile = NULL;
    char command[30];
    int iInfo;

    pFile = fopen(pszInputFile, "r");
    if(pFile == NULL){
        printf("Error opening file\n");
    }
    //if file opened successfully
    else
    {
        //Get line from the input file
        while(fgets(szInputBuffer, 100, pFile) != NULL){
            //break the line down to command and info
            sscanf(szInputBuffer, "%s %d", command, &iInfo);
            // printf("%s %d\n", command, iInfo);
            //if the command was insert, insert into list
            if(strcmp(command, "INSERT") == 0){
                insertNode(ppHead, ppRear, iInfo, 0);
            }
            //if the command is remove, remove first node
            else if(strcmp(command, "REMOVE") == 0){
                *ppHead = removeNode(*ppHead);
            }
            //if neither, then it is an invalid command
            else{
                printf("Invalid command read.  Line skipped\n");
            }
            //print list after each insert or remove
            printList(*ppHead);
        }
    }

    //if file was openend, close it before returning to main
    if(pFile != NULL){
        fclose(pFile);
    }
}

/********************************* makeNode ******************************
NODE *makeNode(int x)
Purpose: To take the integer to be added to the list, create a new node
    dynamically, and to insert the integer into the node, then return the
    new node address.

Parameters:
    I   int x           Integer to be added to the new node

Return:
    NULL        returns NULL if failed to allocate node
    pNew        reutrns address of new node

Notes:
**************************************************************************/
NODE *makeNode(int x){
    NODE *pNew = NULL;
    pNew = (NODE *)malloc(sizeof(NODE));
    if(pNew == NULL){
        printf("error allocating node\n");
        return NULL;
    }else{
        pNew->iInfo = x;
        pNew->pPrev = NULL;
        pNew->pNext = NULL;
        // printf("Allocating Node: Address of pnew: %p\n", pNew);
        return pNew;
    }

}

/********************************* insertNode ******************************
void insertNode(NODE **ppF, NODE **ppR, int x, int iCount)
Purpose: To inser the supplied integer into the linked list, and to update
    the global count of nodes checked before proper insertion

Parameters:
    I/O     NODE **ppF          Pointer to the front of the linked list
    I/O     NODE **ppR          Pointer to the rear of the linked list
    I       int x               Value read from file to be inserted
    I       int iCount          Count of nodes checked

Notes:
**************************************************************************/
void insertNode(NODE **ppF, NODE **ppR, int x, int iCount){
    NODE *pNew = NULL;
    //if front and rear empty, making first node
    if(*ppF == NULL && *ppR == NULL){
        *ppF = makeNode(x);
        // (*ppF)->pNext = NULL;
        // (*ppF)->pPrev = NULL;
        *ppR = *ppF;
        printf("Insert into queue. Checked %d nodes.\n", iCount);
        iTotalChecks = iTotalChecks + iCount;
    }
    //if x is greater than the rear, or smaller than the head, insert
    else if(x <= (*ppF)->iInfo || x >= (*ppR)->iInfo){
        printf ("in insert node if.  x is: %d,  pFinfo is: %d,  pRinfo is: %d\n", x, (*ppF)->iInfo, (*ppR)->iInfo);
        if(x <= (*ppF)->iInfo){
            // printf("adding to front\n");
            pNew = makeNode(x);
            pNew->pNext = *ppF;
            (*ppF)->pPrev = pNew;
            *ppF = pNew;
            //only add one to iCount since we met first criteria
            printf("Insert into queue. Checked %d nodes.\n", iCount +1);
            iTotalChecks = iTotalChecks + iCount + 1;
        }else{
            // printf("adding to end\n");
            pNew = makeNode(x);
            pNew->pPrev = *ppR;
            (*ppR)->pNext = pNew;
            *ppR = pNew;
            printf("Insert into queue. Checked %d nodes.\n", iCount +2);
            iTotalChecks = iTotalChecks + iCount + 2;
        }
    }
    //Must go in the middle since x is guaranteed to be greater than pF and less than pR
    else{
        //set iCount to two since we checked the front and rear node already
        iCount = 2;
        printf ("in insert node else.  x is: %d,  pFinfo is: %d,  pRinfo is: %d\n", x, (*ppF)->iInfo, (*ppR)->iInfo);
        //if x > rear info/2 search from rear
        if(x > ((*ppR)->iInfo)/2){
            NODE *p = *ppR;
            while(p->pPrev != NULL && p->pPrev->iInfo > x){
                // printf(" x>R/2 and ppF->next->next != ppR\n");
                p = p->pPrev;
                iCount++;
            }
            pNew = makeNode(x);
            pNew->pPrev = p->pPrev;
            pNew->pNext = p;
            p->pPrev = pNew;
            pNew->pPrev->pNext = pNew;
            printf("Insert into queue. Checked %d nodes.\n", iCount + 1);
            iTotalChecks = iTotalChecks + iCount;
        }
        //otherwise serach from front
        else{
            NODE *p = *ppF;
            while(p->pNext != NULL && p->pNext->iInfo < x){
                // printf(" x>R/2 and ppF->next->next != ppR\n");
                p = p->pNext;
                iCount++;
            }
            pNew = makeNode(x);
            pNew->pNext = p->pNext;
            pNew->pPrev = p;
            p->pNext = pNew;
            pNew->pNext->pPrev = pNew;
            printf("Insert into queue. Checked %d nodes.\n", iCount+1);
            iTotalChecks = iTotalChecks + iCount + 1;
        }
    }
}

/********************************* removeNode ******************************
NODE *removeNode(NODE *p)
Purpose: To remove the first node in the linked list

Parameters:
    I       NODE *p         Pointer to the front of the linked list

Return:
    pTemp       temp pointer to the second node, to set equal as the new head
    NULL        if there was no node following the current head

Notes:
**************************************************************************/
NODE *removeNode(NODE *p){
    printf("Removing first node from queue.\n");
    if(p->pNext != NULL){
        // printf("p->pNext = %p\n", p->pNext);
        NODE *pTemp = p->pNext;
        // printf("pTemp = %p\n", pTemp);
        free(p);
        return pTemp;
    }
    free(p);
    return NULL;
}

/********************************* printList ******************************
void printList(NODE *p)
Purpose: To print every node currently in the list

Parameters:
    I   NODE *p     Pointer to the head of the list

Notes:
**************************************************************************/
void printList(NODE *p){
    printf("Queue contains: ");
    while(p != NULL){
        printf("%d ", p->iInfo);
        p=p->pNext;
    }
    printf("\n");
}

/********************************* freeNodes ******************************
void freeNodes(NODE *p)
Purpose: To free all currently allocated nodes

Parameters:
    I   NODE *p     Pointer to the head of the linked list.

Notes:
**************************************************************************/
void freeNodes(NODE *p){
    if(p->pNext != NULL)
        freeNodes(p->pNext);
    free(p);
}