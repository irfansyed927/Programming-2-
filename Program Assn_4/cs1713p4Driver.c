/**********************************************************************
cs1713p4Driver.c
Purpose:
    This program reads flight information and a command file.   It 
    processes the commands against the flight information which is 
    represented by a linked list.
    This is the driver code for the routines written by the students.
Command Parameters:
    p4 -f flightFileName -c commandFileName
Input:
    Flight   Stream input file which contains many records defining flights:
                 szFlightId szFrom  szDest  szDepartTm  iAvailSeats dSeatPrice 
                 10s        3s      3s      5s          4d          10lf 

    Command  This is the same as program #3.  The file contains 
             text in the form of commands (one command per text line):  
                 CUSTOMER BEGIN cGender   szBirthDt   szEmailAddr    szFullName
                     specifies the beginning of customer request and includes 
                     all the identification information from program 2.
                 CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
                     specifies the address for a customer (separated by commas)
                 CUSTOMER REQUEST szFlightId iNumSeats
                    specifies a single flight request.  Steps:
                     >	Print the flight ID and requested number of seats
                     >	Lookup the flight ID using a linear search.  If not found, 
                        print a warning (but do not terminate your program) and return.
                     >	If found, try to satisfy the entire requested number of seats.  
                        If not enough seats,  print a warning and return.
                     >	Print the unit price and cost.
                     >	Accumulate the total cost for this customer
                 CUSTOMER COMPLETE
                     specifies the completion of the list of flight requests 
                     for a customer.
                 FLIGHT INCREASE szFlightId iQuantity
                     increase the available seats for a flight by the specified quantity.
                 FLIGHT SHOW szFlightId    
                     requests a display of a particular flight.  Show all of its information.
                 FLIGHT NEW szFlightId szFrom  szDest  szDepartTm  iAvailSeats dSeatPrice

Results:
    Prints the original Flights
    Processes the commands (see above) and shows any errors.
    Prints the resulting Flights
Returns:
    0  normal
    -1 invalid command line syntax
    -2 show usage only
    -3 error during processing, see stderr for more information

Notes:
    p4 -?       will provide the usage information.  In some shells,
                you will have to type p4 -\?

**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p4.h"

// prototypes for this file
void processCommandSwitches(int argc, char *argv[], char **ppszFlightFileName
    , char **ppszCommandFileName);
void processCommands(Node **ppHead, char *pszCommandFileName);
int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[]);

#define MAX_TOKEN_SIZE 20

int main(int argc, char *argv[])
{
    Node        *pHead = NULL;                   // Points to first node in the list
    char        *pszFlightFileName = NULL;       // Flight file name
    char        *pszCommandFileName = NULL;      // Command file name

    processCommandSwitches(argc, argv, &pszFlightFileName, &pszCommandFileName);

    // get the flights and print them
    getFlights(&pHead, pszFlightFileName);
    printFlights("Initial Flights", pHead);
    
    // process the command input file
    processCommands(&pHead, pszCommandFileName);

    // print the flights after processing the command file
    printFlights("Resulting Flights", pHead);
    printf("\n");
    return 0;
}

/******************** processCommands **************************************
    void processCommands(Node **ppHead, char *pszCommandFileName)
Purpose:
    Reads the Command file to process commands.  There are two major
    types of commands:  CUSTOMER and FLIGHT.
Parameters:
    I/O Node **ppHead                 address of pointer to head of LL 
                                     first node in the linked list of flights
    I   char  *pszCommandFileName  Command file name
Notes:
    This opens and closes the Command file.
**************************************************************************/
void processCommands(Node **ppHead, char *pszCommandFileName)
{
    FILE *pfileCommand;                     // stream Input for Customer Customer data
    // variables for command processing
    char szInputBuffer[100];                // input buffer for a single text line
    char szCommand[MAX_TOKEN_SIZE+1];       // command
    char szSubCommand[MAX_TOKEN_SIZE+1];    // subcommand
    int bGotToken;                          // TRUE if getSimpleToken got a token
    int iBufferPosition;                    // This is used by getSimpleToken to 
                                            // track parsing position within input buffer
 
    // variables for customer information (except Flight Request)
    Customer customer;
    double dCustomerRequestTotalCost = 0.0;

    // open the Command stream data file
    if (pszCommandFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-c");

    pfileCommand = fopen(pszCommandFileName, "r");
    if (pfileCommand == NULL)
        exitError(ERR_COMMAND_FILENAME, pszCommandFileName);
    
    /* get command data until EOF
    ** fgets returns null when EOF is reached.
    */
    while (fgets(szInputBuffer, 100, pfileCommand) != NULL)
    {
        printf("%s", szInputBuffer);
        iBufferPosition = 0;                // reset buffer position

        // get the command
        bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szCommand);

        // see what the command is
        if (bGotToken && strcmp(szCommand, "CUSTOMER") == 0)
        {   // CUSTOMER command
            // get the sub comand
            bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szSubCommand);

            if (bGotToken)
                processCustomerCommand(ppHead, szSubCommand
                    , &szInputBuffer[iBufferPosition]           // address past the subcommand
                    , &customer
                    , &dCustomerRequestTotalCost);
            else exitError(ERR_CUSTOMER_SUB_COMMAND, " missing subcommand");
        }
        else  if (bGotToken && strcmp(szCommand, "FLIGHT") == 0)
        {   // FLIGHT command
            bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szSubCommand);
            if (bGotToken)
                processFlightCommand(ppHead, szSubCommand
                    , &szInputBuffer[iBufferPosition]);
            else exitError(ERR_FLIGHT_SUB_COMMAND, " missing subcommand");
        }
        else 
            exitError(ERR_INVALID_COMMAND, szCommand);
    }
    fclose(pfileCommand);
}

/******************** allocateNode **************************************
    Node *allocateNode(Flight flight)
Purpose:
    This allocates a new node and assigns flight to it.  It also
    sets the node's pNext to NULL.
Parameters:
    I Flight flight     a flight which contains information defining a 
                        flight including szFlightId.
Returns:
    A pointer to the newly allocated node..
Notes:
    If the allocation fails, this function exits the program.
**************************************************************************/
Node *allocateNode(Flight flight)
{
    Node *pNew = malloc(sizeof(Node));
    if (pNew == NULL)
        exitError("No memory available for malloc of a flight", "");
    pNew->flight = flight;
    pNew->pNext = NULL;
    return pNew;
}

 /******************** getSimpleToken **************************************
 int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[])
 Purpose:
    Returns the next token in a string.  The delimiter for the token is a 
    space, a newline or the end of the string.  To help with a 
    subsequent call, it also returns the next position in the buffer.
Parameters:
    I   char szInput[]          input buffer to be parsed
    I/O int *piBufferPosition   Position to begin looking for the next token.
                                This is also used to return the next 
                                position to look for a token (which will
                                follow the delimiter).
    O   char szToken[]          Returned token.  
Returns:
    Functionally:
        TRUE - a token is returned
        FALSE - no more tokens
    iBufferPosition parm - the next position for parsing
    szToken parm - the returned token.  If not found, it will be an empty string.
Notes:
    - If the token is larger than the szToken parm, we return a truncated value.
**************************************************************************/

int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[]) 
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n";          // delimiters
    
    // check for past the end of the string
    if (*piBufferPosition >= (int) strlen(szInput))
    {
        szToken[0] = '\0';              // mark it empty
        return FALSE;                   // no more tokens
    }

    // get the position of the first delim, relative to the iBufferPosition
    iDelimPos = strcspn(&szInput[*piBufferPosition], szDelims);

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > MAX_TOKEN_SIZE)
        iCopy = MAX_TOKEN_SIZE;             // truncated size
    else
        iCopy = iDelimPos;
    
    // copy the token into the target token variable
    memcpy(szToken, &szInput[*piBufferPosition], iCopy);
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    *piBufferPosition += iDelimPos + 1;  
    return TRUE;
}

/******************** processCommandSwitches *****************************
    void processCommandSwitches(int argc, char *argv[]
        , char **ppszFlightFileName
        , char **ppszCommandFileName)
Purpose:
    Checks the syntax of command line arguments and returns the filenames.
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                        count of command line arguments
    I   char *argv[]                    array of command line arguments
    O   char **ppszFlightFileName       flight file name
    O   char **ppszCommandFileName      command file name
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/

void processCommandSwitches(int argc, char *argv[], char **ppszFlightFileName
    , char **ppszCommandFileName)
{
    int i;
    int rc = 0;
    int bShowCommandHelp = FALSE;

    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
        case 'f':                   // Flight File Name
            if (++i >= argc)        // There must be an additional argument
                exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
            else
                *ppszFlightFileName = argv[i];
            break;
        case 'c':                   // Customer Customer File Name
            if (++i >= argc)        // There must be an additional argument
                exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
            else
                *ppszCommandFileName = argv[i];
            break;
        case '?':
            exitUsage(USAGE_ONLY, "", "");
            break;
        default:
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
}

/******************** exitError *****************************
    void exitError(char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    Prints an error message and diagnostic to stderr.  Exits with
    ERROR_PROCESSING.
Parameters:
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitError(char *pszMessage, char *pszDiagnosticInfo)
{
    fprintf(stderr, "Error: %s %s\n"
        , pszMessage
        , pszDiagnosticInfo);
    exit(ERROR_PROCESSING);
}

/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    If this is an argument error (iArg >= 0), it prints a formatted message
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits
    with ERR_COMMAND_LINE_SYNTAX.

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/

void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    if (iArg >= 0)
        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
        , iArg
        , pszMessage
        , pszDiagnosticInfo);
    fprintf(stderr, "p4 -f flightFileName -c commandFileName\n");
    if (iArg >= 0)
        exit(-1);
    else
        exit(-2);
}