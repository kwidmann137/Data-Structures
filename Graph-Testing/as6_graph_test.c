/**********************************************************************
CS2123 as6_p3.c by Kyle Widmann
Purpose:
    Read in a file cotainign an Adjacency Matrix with the following format.
    Once read in, the program will store the adjacency matrix, and evaluate the graph for cycles.

Command Parameters:
    -i <datafile.txt>

Input:  The data file for the adjacency matrix must be formated as follows
    <number of vertices>
    <vertex name> <0 or 1> <0 or 1> ... <0 or 1>
    <vertex name> <0 or 1> <0 or 1> ... <0 or 1>
    ....
    <vertex name> <0 or 1> <0 or 1> ... <0 or 1>

    The vertex name must be less than 10 characters

Results:
    The program will print out the adjacency matrix read in from the file and whether or not the graph contains a cycle.

Returns:
    0 - normal
Notes:

**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define vertexSize 4

//typedefs
typedef int bool;

typedef struct vertex{
    char name[10];
    bool visited;
    struct vertex *pParent;
    int adjList[];
}vertex;

typedef struct vertexArray{
    int iCount;
    struct vertex *pVertex[];
}vertexArray;

//function declarations
void readFile(char *pszInputFile, vertexArray **ppAdjMatrix, int *piVertexCount);
void getFile(int argc, char *argv[], char **ppszInputFile);
void makeAdjMatrix(vertexArray **ppArr, int size);
void printAdjMatrix(vertexArray *pAdjMatrix, int iVertexCount);
void testCycle(vertexArray *pAdjMatrix, int iVertexCount);
bool DFS(vertex *pCurr, vertexArray *pAdjMatrix, int iVertexCount);
bool isAncestor(vertex *pParent, vertex *pChild);
vertex* makeVertex(char name[], int size);
void freeVertexArray(vertexArray *pArr);
void freeVertex(vertex *p);


/***************************** MAIN FUNCTION **************************/
int main(int argc, char *argv[])
{
    //variables
    vertexArray *pAdjMatrix=NULL;
    char *pszInputFile = NULL;
    int iVertexCount=0;

    //get filename from CLI
    getFile(argc, argv, &pszInputFile);

    //read file and build adjacency matrix
    readFile(pszInputFile, &pAdjMatrix, &iVertexCount);

    //detemine if graph is cyclic
    testCycle(pAdjMatrix, iVertexCount);

    //free Adj Matrix
    freeVertexArray(pAdjMatrix);

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
void readFile(char *pszInputFile, vertexArray **ppAdjMatrix, int *piVertexCount)
Purpose: read in the provided file and store the adjacency matrix

Parameters:
    I   char *pszInputFile              pointer to the datafile
    I/O vertexArray **ppAdjMatrix       double pointer to the array of vertices
    I/O int *piVertexCount              pointer to the count of vertices

Notes:
***********************************************************************/
void readFile(char *pszInputFile, vertexArray **ppAdjMatrix, int *piVertexCount){
    char szInputBuffer[100], *cToken;
    FILE *pFile = NULL;
    int iLineCount=0, iListCount=0, iCurrentVertex=0;
    vertex *pCurr=NULL;

    pFile = fopen(pszInputFile, "r");
    if(pFile == NULL){
        printf("Error opening file.  Now exiting.\n");
        exit(0);
    }
    else
    {
        while(fgets(szInputBuffer, 100, pFile) != NULL){
            //if this is the first line from the file, it should be our node count
            if(iLineCount == 0){
                if(sscanf(szInputBuffer, "%d", piVertexCount) != 1){
                    printf("Node count was not supplied.\n");
                }
                iLineCount++;
                //allocate space for array of vertex's * node size
                makeAdjMatrix(ppAdjMatrix, *piVertexCount);
            }
            //we are past the first line, so read in each token
            else
            {
                //the first token is the vertex name
                cToken = strtok(szInputBuffer, " ");

                //set pCurr to the next vertex in the vertex Array(AdjMatrix)
                pCurr = (*ppAdjMatrix)->pVertex[iCurrentVertex] = makeVertex(cToken, *piVertexCount);

                //the rest of the tokens make up the vertex's Adj list
                while((cToken = strtok(NULL, " ")) != NULL){
                    //convert token to int and store in adjList for current vertex
                     pCurr->adjList[iListCount] = atoi(cToken);
                    //increase the spot in the adjList we are now at
                    iListCount ++;
                }
                //reset list count to 0 for next vertex
                iListCount = 0;
                //incremement through the array of vertices
                iCurrentVertex++;
            }
        }
    }

    //Print adjacency Matrix
    printAdjMatrix(*ppAdjMatrix, *piVertexCount);

    //close file
    if(pFile != NULL){
        fclose(pFile);
    }
}

/****************************** makeAdjMatrix *******************************
void makeAdjMatrix(vertexArray **pArr, int size)
Purpose: to allocate space for the array of vertices

Parameters:
    I/O  vertexArray **ppArr        double pointer to array of vertices
    I    int size                   count of number of vertices from file

Notes:
***********************************************************************/
void makeAdjMatrix(vertexArray **ppArr, int size){
    *ppArr = (vertexArray *)malloc(sizeof(vertexArray) + sizeof(vertex*) *size );
    (*ppArr)->iCount = size;
}

/****************************** makeVertex *******************************
vertex* makeVertex(char name[], int size)
Purpose:  allocate space to store a vertex's information

Parameters:
    I   char name[]         String containing the vertices name from the datafile
    I   int size            number of vertices, so we know how long to make the
                            adjacency list
Return:
    pVertex                 pointer to the vertex created
    NULL                    returned if failed to allocate space for vertex

Notes:
***********************************************************************/
vertex* makeVertex(char name[], int size){
    int i;
    //allocate vertex
    vertex *pVertex = (vertex *)malloc(sizeof(vertex) + sizeof(int)*size);
    if(pVertex != NULL){
        strcpy(pVertex->name, name);
        pVertex->pParent = NULL;
        pVertex->visited = FALSE;
        //initialize vertex Adj list to 0;
        for(i=0; i<size; i++){
            pVertex->adjList[i]=0;
        }
        return pVertex;
    }
    return NULL;
}

/****************************** printAdjMatrix *******************************
void printAdjMatrix(vertexArray *pAdjMatrix, int iVertexCount)
Purpose:
    Print out the adjacency matrix stored from the datafile

Parameters:
    I       vertexArray *pAdjMatrix         pointer to the array of vertices
    I       int iVertexCount                Count of vertices

Notes:
***********************************************************************/
void printAdjMatrix(vertexArray *pAdjMatrix, int iVertexCount){
    int i, j;
    vertex *pCurr=NULL;
    printf("Printing Adjacnecy Matrix\n\n");
    for(i=0; i<iVertexCount; i++){
        pCurr = pAdjMatrix->pVertex[i];
        printf("%s ", pCurr->name);
        for(j=0; j<iVertexCount; j++){
            printf("%d ", pCurr->adjList[j]);
        }
        printf("\n");
    }
}

/****************************** testCycle *******************************
void testCycle(vertexArray *pAdjMatrix, int iVertexCount)
Purpose:
    To test if the supplied array of vertices has a cycle resulting from a backedge

Parameters:
    I   vertexArray *pAdjMatrix         pointer to array of vertices to be tested
    I   int iVertexCount                count of vertices in array

Notes:
***********************************************************************/
void testCycle(vertexArray *pAdjMatrix, int iVertexCount){
    //variables
    vertex *pCurr = pAdjMatrix->pVertex[0];       //ptr to current vertex being analyzed
    bool cycle = FALSE;                             //Boolean for cycle detected
    int i=0;

    //while no cycle detected send the next unvisited node to DFS to check for cycle
    while(cycle == FALSE && pCurr->visited == FALSE){
        //send it to DFS
        cycle = DFS(pCurr, pAdjMatrix, iVertexCount);
        //if DFS comes back true then there is a cycle and loop will break
        //otherwise increment ptr to next univisited node to send to DFS
        if(cycle == FALSE){
            //use i to ensure we don't go past our array of vertex's
            //if the current vertex was visited, increment again until we find our an unvisited vertex or we hit our second to last vertex, since we increment i after entering the loop
            while((i < iVertexCount-1) && (pCurr->visited == TRUE)){
                i++;
                pCurr = pAdjMatrix->pVertex[i];
            }
        }
    }

    if(cycle == TRUE){
        printf("\nThis graph contains a cycle.\n");
    }
    else{
        printf("\nThe graph does not contain a cycle.\n");
    }
}


/****************************** DFS *******************************
bool DFS(vertex *pCurr, vertexArray *pAdjMatrix, int iVertexCount)
Purpose:
    To go through a depth-first-search of the supplied array of verticies, starting at the vertex supplied through pCurr.  If a cycle is detected via a back edge, return True, otherwise return False

Parameters:
    I   vertex *pCurr               Pointer to starting vertex
    I   vertexArray *pAdjMatrix     pointer to array of verticies
    I   int iVertexCount            Count of verticies in array

Return:
    TRUE                Returns true of a back edge was identified
    FALSE               Returns false if no backe edge was found and all child verticies
                        have been searched

Notes:
***********************************************************************/
bool DFS(vertex *pCurr, vertexArray *pAdjMatrix, int iVertexCount){
    //mark current vertex as visiited
    pCurr->visited = TRUE;

    //VARIABLES
    vertex *pAdjVertex = NULL;
    int i=0;

    //Loop through adjacency list
    while(i<iVertexCount){

        //if the current adjacency list item represents an adjacent vertex
        if(pCurr->adjList[i] == 1){

            //set AdjVertex ptr equal to the adjVertex from Adj List
            pAdjVertex = pAdjMatrix->pVertex[i];

            //set AdjVertex parent equal to the current vertex
            pAdjVertex->pParent = pCurr;

            //if we have not visited the Adj Vertex, investigate
            if(pAdjVertex->visited == FALSE){
                if(DFS(pAdjVertex, pAdjMatrix, iVertexCount) == TRUE){
                    return TRUE;
                }
            }
            //if we have visited the Adj Vertex and it is Ancestor, we have a cycle
            if(pAdjVertex->visited == TRUE && isAncestor(pAdjVertex, pCurr) == TRUE){
                return TRUE;
            }
        }
        //set the Adj Vertex to NULL until next Adj Vertex is pulled form current vertex Adj List
        pAdjVertex = NULL;
        //increment our spot in the current vertex's Adj List
        i++;
    }

    return FALSE;
}

/****************************** isAncestor *******************************
bool isAncestor(vertex *pParent, vertex *pChild)
Purpose:
    to determined if the vertex pParent is actually a parent of vertex pChild

Parameters:
    I   vertex *pParent         pointer to the potential parent vertex
    I   vertex *pChild          pointer to the potential child vertex

Notes:
***********************************************************************/
bool isAncestor(vertex *pParent, vertex *pChild){
    while(pChild->pParent != NULL){
        if(pChild->pParent == pParent){
            return TRUE;
        }
        pChild = pChild->pParent;
    }

    return FALSE;
}

/****************************** freeVertexArray **************************
void freeVertexArray(vertexArray *pArr)
Purpose:
    to free all verticies in the array, and finally the array itself

Parameters:
    I   vertexArray *pArr       pointer to array of verticies

Notes:
***********************************************************************/

void freeVertexArray(vertexArray *pArr){
    int i;
    for(i = 0; i<pArr->iCount; i++){
        freeVertex(pArr->pVertex[i]);
    }
    free(pArr);
}

/****************************** freeVertex *******************************
void freeVertex(vertex *p)
Purpose:
    free the vertex supplied

Parameters:
    I   vertex *p           pointer to vertex to be freed

Notes:
***********************************************************************/

void freeVertex(vertex *p){
    free(p);
}