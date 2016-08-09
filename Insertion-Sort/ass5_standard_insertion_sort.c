/**********************************************************************
CS2123 ass5_p2.c by Kyle Widmann
Purpose:
    Perform a standard insertion sort on the integers supplied via the data file.

Command Parameters:
    ass5_p2 -i <datafile.txt>
Input:
    Read in a data file contianing a list of integers
Results:
    Take the integers from the supplied file, store them in an array, then using insertion sort, put the elements in increasing order.
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
void insertSort(int iA[], int iLength);

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
            // printf("%d\n", iInfo);
        }
        // printf("Total Elements: %d\n", iElem);

        //make an array size for the elements
        int iArray[iElem];

        iElem = 0;
        rewind(pFile);
        //loop through to add integers to array
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
        insertSort(iArray, iElem);
    }

    if(pFile != NULL){
        fclose(pFile);
    }
}

/****************************** insertSort *****************************
void insertSort(int iA[], int iLength)
Purpose: Do a standard insertion sort in the array of integers supplied

Parameters:
    I/O    int iA[]        unsorted array of integers from data file, to be sorted and returned via parameters
    I      int iLength     Length of the array

Notes:
***********************************************************************/
void insertSort(int iA[], int iLength)
{
    int i, j, key, assignments = 0;

    //for every element > index 0
    for (i = 1; i<iLength; i++)
    {
        //save iA[i] value since we might shift values right
        key = iA[i];
        j = i-1;

        //shift all elements > key to the right
        while(j >= 0 && key < iA[j])
        {
            iA[j+1] = iA[j];
            assignments ++;
            j--;
        }
        //insert key if j changed
        if( j != i-1)
        {
            iA[j+1] = key;
            assignments ++;
        }
    }

    //print sorted array
        printf("Printing sorted array\n");
        for(i = 0; i<iLength; i++)
        {
            printf("%d  ", iA[i]);
        }
        printf("\n");

    printf("Total number of assignments: %d\n", assignments);
}