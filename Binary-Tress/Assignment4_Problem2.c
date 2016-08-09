/**********************************************************************
CS2123 assignment4_Problem2.c by Kyle Widmann
Purpose:
    This program generates a binary tree of 100 random numbers.  This process is repeated 50 times.  With each new tree, the level difference from highest to lowest node is recorded and stored in a separate binary tree.  This information is then used to report the imbalance of the trees across the experiment.

Results:
    The program will output a list showing the level difference, and how many of the 50 runs had that particular level difference.  Only level differences with at least 1 run will be shown.
Returns:
    0 - normal
Notes:

**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Structure to store randomly generated numbers
typedef struct node{
    int i;
    struct node* pLeft;
    struct node* pRight;
}node;

//structure to store info about each iteration/run
typedef struct diffNode{
    int iDiff;
    int iRuns;
    struct diffNode* pLeft;
    struct diffNode* pRight;
}diffNode;

//Functions
int generateRandom();
void prettyPrintList(node *pRoot, int i);
node *insertNode(node *pRoot, node *pNew);
node *makeNode();
void generateList(node **pRoot, diffNode **pDiffRoot);
void freeList(node *pRoot);
int countHighest(node *pRoot);
int countLowest(node *pRoot);
diffNode *makeDiffNode();
void insertDiffTree(diffNode **pR, int iDiff);
int printDiffList(diffNode *pRoot);
void freeDiffList(diffNode *pRoot);


/*********** MAIN FUNCTIION ***************/
int main(int argc, char *argv[]){
    //variables used for for-loop and to store total runs
    int i, iTotalRuns;

    //establish root for diffNode to store run info
    diffNode *pDiffRoot = NULL;

    //establish seed for random number generatore
    srand(time(NULL));

    //generate list of random numbers 50 times;
    for(i = 0; i<50; i++){
        node *pRoot = NULL;
        generateList(&pRoot, &pDiffRoot);
        //free tree after each iteration
        freeList(pRoot);
    }

    //print level differences and number of runs
    printf("\nSUMMARY\n");
    printf("Level Difference\t# Runs\n");
    iTotalRuns = printDiffList(pDiffRoot);
    printf("\t\t\t__\nTotal Runs:\t\t%d\n", iTotalRuns);
    printf("\nNote: Only level differences with at least 1 qualifying run are shown.\n");

    //free diffTree
    freeDiffList(pDiffRoot);

    return 0;
}

void generateList(node **pRoot, diffNode **pDiffRoot){
    node *pTemp = NULL;
    int i, iHighest, iLowest;

    //create 100 random numbers and insert into tree;
    for(i = 0; i<100; i++){
        //create node with random number
        pTemp = makeNode();
        //insert node into tree
        *pRoot = insertNode(*pRoot, pTemp);
    }

    iHighest = countHighest(*pRoot);
    iLowest = countLowest(*pRoot);
    // prettyPrintList(*pRoot, 0);
    //print level differences
    printf("Highest node level: %d  Lowest: %d  Total difference: %d\n", iHighest, iLowest, iLowest-iHighest);
    insertDiffTree(pDiffRoot, (iLowest-iHighest));
}

node *makeNode(){
    node *pTemp = NULL;
    int i;

    i = generateRandom();
    // printf("%d ,", i);

    pTemp = (node *)malloc(sizeof(node));
    pTemp->i = i;
    pTemp->pLeft = NULL;
    pTemp->pRight = NULL;

    return pTemp;
}

diffNode *makeDiffNode(){
    diffNode *pTemp = NULL;

    pTemp = (diffNode *)malloc(sizeof(diffNode));
    pTemp->iDiff = 0;
    pTemp->iRuns = 0;
    pTemp->pLeft = NULL;
    pTemp->pRight = NULL;

    return pTemp;
}

node *insertNode(node *pRoot, node *pNew){
    if(pRoot == NULL){
        pRoot = pNew;
        return pRoot;
    }else if(pNew->i < pRoot->i){
        pRoot->pLeft = insertNode(pRoot->pLeft, pNew);
    }else{
        pRoot->pRight = insertNode(pRoot->pRight, pNew);
    }
    return pRoot;
}

void insertDiffTree(diffNode **pR, int iDiff){
    //if hit null, make new node
    if(*pR == NULL){
        *pR = makeDiffNode();
        (*pR)->iDiff = iDiff;
        (*pR)->iRuns = 1;
    }
    else if(iDiff == (*pR)->iDiff){
        (*pR)->iRuns++;
    }
    else if(iDiff < (*pR)->iDiff){
        insertDiffTree(&(*pR)->pLeft, iDiff);
    }else {
        insertDiffTree(&(*pR)->pRight, iDiff);
    }
}

int generateRandom() {
    return rand() % 1000 + 1;
}

int printDiffList(diffNode *pRoot){
    int sum;
    if(pRoot == NULL){
        return 0;
    }
    sum = printDiffList(pRoot->pLeft);
    printf("%d\t\t\t%d\n", pRoot->iDiff, pRoot->iRuns);
    sum = sum + printDiffList(pRoot->pRight);
    return sum + pRoot->iRuns;
}

void prettyPrintList(node *pRoot, int i){
    if(pRoot == NULL){
        return;
    }
    prettyPrintList(pRoot->pRight, i+1);
    printf("%*c %d    level: %d\n", i, ' ', pRoot->i, i);
    prettyPrintList(pRoot->pLeft, i+1);
}

int countHighest(node *pRoot){
    int l, r;
    if (pRoot == NULL)
        return -1;
    l = countHighest(pRoot->pLeft);
    r = countHighest(pRoot->pRight);
    // printf("l: %d  r: %d number: %d\n", l, r, pRoot->i);
    return (l < r) ? l+1 : r+1;
}

int countLowest(node *pRoot){
    int l, r;
    if (pRoot == NULL)
        return -1;
    l = countLowest(pRoot->pLeft);
    r = countLowest(pRoot->pRight);
    // printf("l: %d  r: %d number: %d\n", l, r, pRoot->i);
    return (l > r) ? l+1 : r+1;
}

void freeList(node *pRoot){
    if( pRoot == NULL){
        return;
    }
    freeList(pRoot->pLeft);
    freeList(pRoot->pRight);
    free (pRoot);
}

void freeDiffList(diffNode *pRoot){
    if(pRoot == NULL){
        return;
    }
    freeDiffList(pRoot->pLeft);
    freeDiffList(pRoot->pRight);
    free (pRoot);
}