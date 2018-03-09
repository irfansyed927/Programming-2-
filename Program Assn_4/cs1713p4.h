/**********************************************************************
cs1713p4.h
Purpose:
    Defines typedefs for
        Customer      - includes email address, full name, birthdate, gender,
                        and postal address
        FlightRequest - flights the customer wants.
        Flight        - flight information
        Node          - a node in a ordered singly linked list
    Defines constants for
        boolean values
        error messages
        program return codes
    Prototypes
Notes:
    
**********************************************************************/

/**** typedefs ****/

// Customer information 
typedef struct
{
    char        cGender;                    // M - Male or F - Female
    char        szBirthDt[11];              // Birthdate - YYYY/MM/DD
    char        szEmailAddr[51];            // customer's email address is used
                                            // to uniquely define a customer
    char        szFullName[31];             // customer full name
    char        szStreetAddress[31];        // street number, street name, and 
                                            // (optional) apartment number
    char        szCity[21];                 // address city
    char        szStateCd[3];               // address state code
    char        szZipCd[6];                 // address zip code
} Customer;

// Flight Request information 
typedef struct
{
    char szFlightId[11];                   // Flight Identifier 
    int iRequestSeats;                     // Requested number of seats
} FlightRequest;

// Flight Definition
typedef struct
{
    char szFlightId[11];                    // Flight Identifier
    char szFrom[4];                         // Origin of the flight
    char szDest[4];                         // Destination of the flight
    char szDepartTm[6];                     // Departure Time (24 hour clock) HH:MM
    int  iAvailSeatCnt;                     // Number of available seats
    double dSeatPrice;                      // Price per seat (i.e., unit price)
} Flight;
/* Node is used for one node in a linked list
*/
typedef struct Node
{
    Flight   flight;                        // contains flight information
    struct Node *pNext;                     // next node in a linked list
} Node;

/**** constants ****/

// boolean
#define FALSE           0
#define TRUE            1

//  Error Messages 
#define ERR_MISSING_SWITCH          "missing switch"
#define ERR_EXPECTED_SWITCH         "expected switch, found "
#define ERR_MISSING_ARGUMENT        "missing argument for "
#define ERR_COMMAND_FILENAME        "invalid command file name "
#define ERR_CUSTOMER_ID_DATA        "invalid customer id data "
#define ERR_CUSTOMER_ADDRESS_DATA   "invalid customer address data "
#define ERR_RESERVATION_DATA        "invalid customer reservation data "
#define ERR_FLIGHT_FILENAME         "invalid flight definition file name "
#define ERR_FLIGHT_DEF_DATA         "invalid flight definition data "
#define ERR_TOO_MANY_FLIGHTS        "attempted to put more than MAX_FLIGHTS in the flight array, found "
#define ERR_FLIGHT_NOT_FOUND        "flight ID not found "
#define ERR_TOO_FEW_SEATS           "not enough available seats to satisfy request "
#define ERR_CUSTOMER_SUB_COMMAND    "invalid CUSTOMER subcommand "
#define ERR_FLIGHT_SUB_COMMAND      "invalid FLIGHTsubcommand "
#define ERR_INVALID_COMMAND         "invalid command "
#define ERR_INCREASE_SUB_COMMAND    "invalid INCREASE subcommand "
#define ERR_SHOW_SUB_COMMAND        "invalid SHOW subcommand "
#define ERR_FLIGHT_EXISTS           "flight already exists"

// program return codes
#define ERR_COMMAND_LINE_SYNTAX     -1      // invalid command line syntax
#define USAGE_ONLY                  -2      // show usage only
#define ERROR_PROCESSING            -3

/**** prototypes ****/

// functions you must code
void printFlights(char *pszHeading, Node *pHead);
void processCustomerCommand(Node **ppHead
    , char *pszSubCommand, char *pszRemainingInput
    , Customer *pCustomer, double *pdCustomerRequestTotalCost);
void processFlightCommand(Node **ppHead
    , char *pszSubCommand, char *pszRemainingInput);

/******************** searchLL *****************************
   Node *searchLL(Node *pHead, char szMatchFlightId[]) 
Purpose:
    
Parameters:
    I   Node *pHead                    Head of list pointer
    I   char szMatchFlightId[]         Flight Id to find in list
Returns:
    pointer to the node
    NULL if not found
Notes:
   from the SimpleLL notes, but you must change slightly
**************************************************************************/
Node *searchLL(Node *pHead, char szMatchFlightId[]);


/******************** insertLL *****************************
  Node *insertLL(Node **ppHead, Flight flight) 
Purpose:
    
Parameters:
    I   Node **pHead                    address of Head of list pointer
    I   char char szMatchFlightId[]     Flight Id to find in the tree
Returns: 
    pointer to the node                 if NULL, must allocate memory
                                         to hold the flight and return
                                         its pointer 
    NULL if flight is invalid 
Notes:
   from the SimpleLL notes, but you must change slightly
**************************************************************************/
Node *insertLL(Node **ppHead, Flight flight); 


// Functions Larry provided
Node *allocateNode(Flight flight);
int getFlights(Node **ppHead, char * pszFlightFileName);

// Error handling functions
void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);