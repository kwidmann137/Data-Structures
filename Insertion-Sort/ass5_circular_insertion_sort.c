/**********************************************************************
CS2123 ass5_p3.c by Kyle Widmann
Purpose:
    Read in the integers from the data file, and then sort them using a circular array.

Command Parameters:
    ass5_p3 -i <datafile.txt>
Input:
    Read in a data file contianing a list of integers
Results:
    Take the integers, put them into an inital array for storage, then using a circular sort algorithm, sort them in a circular array.
Returns:
    0 - normal
Notes:

**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//function declarations
void getFile(int argc, char *argv[], char **ppszInputFile);
void readFile(char *pszInputFile);
void circularSort(int iA[], int iLength);
int insertLeft(int iSorted[], int *iHead, int iMid, int iLength, int key);
int insertRight(int iSorted[], int *iTail, int iMid, int iLength, int key);
int calculateMid(int iHead, int iTail, int iLength);


/***************************** MAIN FUNCTION **************************/
int main(int argc, char *argv[])
{
    char *pszInputFile = NULL;

    getFile(argc, argv, &pszInputFile);

    readFile(pszInputFile);

    return 0;
}

/******************************* getFile ******************************
void getFile(int argc, char *argv[], char **ppszInputFile)
Purpose:
    Reads the CLI to assign file name containing student info to pszInputFile
Parameters:
    I   int argc                    the count of command line arguments
    I   char argv[]                 Array of the command line arguments
    I/O char **ppszInputFile        Double pointer to character string for the file name
Notes:
***********************************************************************/

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
        case 'i':                   // data file name
            if (++i >= argc)
                printf("Error: Missing Switch\n");
            else
                *ppszInputFile = argv[i];
            break;
        default:
            printf("Error: Switch expected");
        }
    }
}


/****************************** readFile *******************************
void readFile(char *pszInputFile)
Purpose:  Read in the integers from the file supplied, store them, then sort them.

Parameters:
    I   char *pszInputFile      Pointer to data file from CLI

Notes:
***********************************************************************/
void readFile(char *pszInputFile){
    char szInputBuffer[100];
    FILE *pFile = NULL;
    int iInfo, iElem=0, i;

    pFile = fopen(pszInputFile, "r");
    if(pFile == NULL){
        printf("Error opening file\n");
    }
    else
    {
        //loop through to figure out how long the list is.
        while(fgets(szInputBuffer, 100, pFile) != NULL){
            iElem += sscanf(szInputBuffer, "%d", &iInfo);
        }

        //make an array size for the elements
        int iArray[iElem];

        iElem = 0;
        rewind(pFile);
        //loop through to add integers to array.
        while(fgets(szInputBuffer, 100, pFile) != NULL){
            if(sscanf(szInputBuffer, "%d", &iInfo) == 1){
                iArray[iElem] = iInfo;
                iElem ++;
            }
        }

        //print unsorted array
        printf("Printing unsorted array\n");
        for(i = 0; i<iElem; i++)
        {
            printf("%d  ", iArray[i]);
        }
        printf("\n");

        //insert sort the elements
        circularSort(iArray, iElem);

    }

    if(pFile != NULL){
        fclose(pFile);
    }
}

/**************************** circularSort *****************************
void circularSort(int iA[], int iLength)
Purpose:  Take the unsorted elemenst from iA and using a two-way insertion sort on a circula array, sort and insert the elements

Parameters:
    I   int iA[]        unsorted array of integers form data file
    I   int iLength     Length of iA

Notes:
***********************************************************************/
void circularSort(int iA[], int iLength)
{
    int iSorted[iLength];
    int iHead, iTail, key, iMid, iElems =1, assignments = 0, i;

    //set head and tail to middle of array
    iHead = iTail = iLength/2;

    //set middle of sorted array to first elem in iA
    iSorted[iHead]=iA[0];

    //cycle through remaining elements in iA
    while(iElems < iLength)
    {
        //set key to element to be inserted
        key = iA[iElems];

        if(key <= iSorted[iHead])
        {
            iHead --;
            //if head has run off front, wrap
            if(iHead == -1){
                iHead = iLength-1;
            }
            iSorted[iHead] = key;
            assignments++;
        }
        else if (key >= iSorted[iTail])
        {
            iTail ++;
            //if tail has run off the end, wrap
            if(iTail > iLength -1){
                iTail = 0;
            }
            iSorted[iTail] = key;
            assignments++;
        }
        else
        {
            //key goes between head and tail, find which is closer
            //calculate middle index
            iMid = calculateMid(iHead, iTail, iLength);

            //if key is smaller than mid point of array
            if(key < iSorted[iMid])
            {
                // shift left and insert
                assignments += insertLeft(iSorted, &iHead, iMid, iLength, key);
            }
            //key was larger than mid point
            else
            {
                // shift right and insert
                assignments += insertRight(iSorted, &iTail, iMid, iLength, key);
            }
        }
        iElems ++;
    }



    //print sorted array
        printf("Printing sorted array\n");
        printf("iHead = %d  iTail = %d\n", iHead, iTail);
        i = iHead;
        printf("%d  ", iSorted[iHead]);
        while(i != iTail){
            i++;
            if(i == iLength)
                i = 0;
            printf("%d  ", iSorted[i]);
        }
        printf("\n");
        printf("Total assignments: %d\n", assignments);
}

/****************************** insertLeft *****************************
int insertLeft(int iSorted[], int *iHead, int iMid, int iLength, int key)
Purpose: Find the index left of the mid point to insert the key, shift all elements past that index to the left 1 spot to make room, then insert the key into the index position

Parameters:
    I/O     int iSorted[]       The sorted array to insert key into
    I/O     int *iHead          Current head position, updated via pointer
    I       int iMid            Current middle position between head and tail
    I       int iLength         Length of array
    I       int key             key to be inserted into iSorted

Return:
    int assignments     Total assignments made shifting elements and inserting the key for inserting this key

Notes:
***********************************************************************/
int insertLeft(int iSorted[], int *iHead, int iMid, int iLength, int key)
{
    int i, j, assignments = 0;

    // printf("in insertLeft.  iHead = %d, key =  %d, iMid = %d\n", *iHead, key, iMid);

    // for every element > index 0
    for (i = iMid; i != *iHead -1; i--)
    {
        // printf("i = %d, iHead = %d\n", i, *iHead);
        //i moved out of bounds so circle back to end of array
        if(i == -1){
            i = iLength-1;
        }

        //compare current index with key
        if( key > iSorted [i]){
            // key goes at current i position, so move everything to the left
            for(j = *iHead-1; j != i ; j++)
            {
                if(j >= iLength-1 || j == -1){
                    //went off end of array, set to 0, will be 1 after incremented
                    if(j == iLength -1)
                        j = 0;
                    iSorted[iLength-1] = iSorted[0];
                    assignments++;
                }
                else{
                    iSorted[j] = iSorted[j+1];
                    assignments++;
                }
            }

            //iHead moved back so decrement iHead
            *iHead = *iHead - 1;

            if(*iHead == -1)
                *iHead = iLength-1;

            //all elements shifted left, now insert key at i
            iSorted[i] = key;
            assignments++;

            //key was inserted so stop looping through array
            break;
        }
    }
    return assignments;
}

/****************************** insertRight ****************************
int insertRight(int iSorted[], int *iTail, int iMid, int iLength, int key)
Purpose:  Find the index right of the mid point to insert the key, shift all elements past that index to the right 1 spot to make room, then insert the key into the index position

Parameters:
    I/O     int iSorted[]       The sorted array to insert key into
    I/O     int *iTail          Current tail position, updated via pointer
    I       int iMid            Current middle position between head and tail
    I       int iLength         Length of array
    I       int key             key to be inserted into iSorted


Return:
    int assignments     Total assignments made shifting elements and inserting the key for inserting this key


Notes:
***********************************************************************/
int insertRight(int iSorted[], int *iTail, int iMid, int iLength, int key)
{
    int i, j, assignments = 0;

    //for every element > index 0
    for (i = iMid; i != *iTail + 1; i++)
    {
        //moved out of bounds, set to begining of array
        if(i == iLength){
            i = 0;
        }

        //compare key to current index
        if(key < iSorted[i]){
            //key goes in current i position
            //move everything right
            for(j = *iTail+1; j != i ; j--)
            {
                //key goes at current i position so shift everything right
                if(j == iLength || j == -1){
                    //went off start of array, set to end of array, will be iLength-1 after decrement
                    if(j == -1)
                        j = iLength;
                    iSorted[0] = iSorted[iLength-1];
                    assignments++;
                }
                else{
                    iSorted[j] = iSorted[j-1];
                    assignments++;
                }
            }

            //increment tail since moved everything right
            *iTail = *iTail + 1;
            if(*iTail == iLength)
                *iTail = 0;

            //all elements shifted, insert key into i
            iSorted[i] = key;
            assignments++;

            //key was inserted so stop looping through array
            break;
        }
    }
    return assignments;
}

/****************************** calculateMid ***************************
int calculateMid(int iHead, int iTail, int iLength)
Purpose:  Calculates the current middle index between iHead and iTail

Parameters:
    I   int iHead           Current position of iHead
    I   int iHead           Current position of iTail
    I   int iLength         Lenght of array

Return:
    int iMid       Current middle index between iHead and iTail

Notes:
***********************************************************************/
int calculateMid(int iHead, int iTail, int iLength)
{
    int iMid;
    if(iTail - iHead > 0){
        //if neither head or tail has wrapped around
        iMid = iHead + (iTail - iHead)/2;
    }else{
        //head or tail has wrapped
        //iHead-iTail = unused elements to subtract from total length
        iMid = iTail - (iLength - (iHead-iTail))/2;
    }
    return iMid;
}