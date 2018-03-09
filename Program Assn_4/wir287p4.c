/***********************************************************************************
cs1713p4.c by your name   (skeletal version)
Purpose:
    This program reads flight information and a command file.   It 
    processes the commands against the flight information.
    This file contains the functions that students write.
Command Parameters:
    p4 -f flightFileName -c commandFileName
Input:
    Flight   Stream input file which contains many records defining flights:
                 szFlightId szFrom  szDest  szDepartTm  iAvailSeats dSeatPrice 
                 10s        3s      3s      5s          4d          10lf 

    Command  This is different from the previous assignment.  The file contains 
             text in the form of commands (one command per text line):  
                 CUSTOMER BEGIN cGender   szBirthDt   szEmailAddr    szFullName
                     specifies the beginning of customer request and includes 
                     all the identification information from program 2.
                 CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
                     specifies the address for a customer (separated by commas)
                 CUSTOMER REQUEST szFlightId iNumSeats
                     specifies a single flight request.  Steps:
                     >	Print the flight ID and requested number of seats
                     >	Lookup the flight ID using a binary search.  If not found,
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

Results:
    Prints the Flights prior to sorting
    Prints the Flight after sorting.
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
void printFlights(char *pszHeading, Node *pHead){
 
    Node *p;
    printf("%s\n", pszHeading);
    printf("    %-10s   %-4s %-4s %-6s %-6s  %-10s\n"
        , "Flight Id", "From", "Dest", "Depart", "Avail", "Unit Price");

    for(p = pHead; p != NULL; p = p->pNext)
    {
        printf("    %-10s   %-4s %-4s %-6s %5d   %10.2lf\n"   
        , p->flight.szFlightId
        , p->flight.szFrom
        , p->flight.szDest    
        , p->flight.szDepartTm
        , p->flight.iAvailSeatCnt
        , p->flight.dSeatPrice);
    }
    printf("\n");
}
   


int getFlights(Node **ppHead, char * pszFlightFileName){
    char szInputBuffer[100];      // input buffer for reading data
    
    int iScanfCnt;                  // returned by sscanf
    FILE *pFileFlight;              // Stream Input for Flights data. 
    Flight flight; 
    
    /* open the Flights stream data file */
    if (pszFlightFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-f");

    pFileFlight = fopen(pszFlightFileName, "r");
    if (pFileFlight == NULL)
        exitError(ERR_FLIGHT_FILENAME, pszFlightFileName);

    /* get flight information until EOF
    ** fgets returns null when EOF is reached.
    */
   // read customer info until EOF
   while (fgets(szInputBuffer, 100, pFileFlight) != NULL){   
     iScanfCnt = sscanf(szInputBuffer, "%s %s %s %s %i %lf\n",
                           flight.szFlightId,
                           flight.szFrom,
                           flight.szDest,
                           flight.szDepartTm,
                           &flight.iAvailSeatCnt,
                           &flight.dSeatPrice);
  
      if(iScanfCnt < 6)
         exitError(ERR_FLIGHT_DEF_DATA, szInputBuffer);
      insertLL(ppHead, flight);
      
   }
   return 0;
   fclose(pFileFlight);
}
   Node *searchLL(Node *pHead, char szMatchFlightId[]) 
{
    Node *p;
     
    for(p = pHead; p != NULL; p = p->pNext)
    {	
	if(strcmp(szMatchFlightId, p->flight.szFlightId) == 0)
            return p;
        if(strcmp(szMatchFlightId, p->flight.szFlightId) < 0)
            return NULL;    
    }
    return NULL;
}


Node *insertLL(Node **ppHead, Flight flight)
{
	
    	Node *pFound;
    	Node *pNew;
	Node *pPrecedes = NULL;
	pNew = allocateNode(flight);
	if(*ppHead == NULL){
		*ppHead = pNew;
		return pNew;}
	Node *p; 
	p = *ppHead;

	while(p != NULL){
		if(strcmp(p->flight.szFlightId, flight.szFlightId) == 0){
			printf("\t%s\t%s\t%s\t%s\t%d\t%lf\n",  pNew -> flight.szFlightId,
                                                       pNew -> flight.szFrom, 
                                                       pNew -> flight.szDest, 
                                                       pNew -> flight.szDepartTm,
                                                       pNew -> flight.iAvailSeatCnt,
                                                       pNew -> flight.dSeatPrice);
        	    	printf("\t\t\t\t\t\t*** flight already exists\n");
			return NULL;
		}else if(strcmp(p->flight.szFlightId, flight.szFlightId) > 0){
			if(pPrecedes == NULL){
				pNew->pNext = *ppHead;
				*ppHead = pNew;
				return pNew;

			}else{
				pNew->pNext = pPrecedes->pNext;
				pPrecedes->pNext = pNew;
				return pNew;
			}
		}else{
			pPrecedes = p;
			p = p->pNext;
		}
	}

	pPrecedes->pNext = pNew;
	return pNew;
}
/*

	for(pFound = *ppHead; pFound != NULL; pFound = pFound->pNext){
	
	printf("%s\n", pFound->flight.szFlightId);	
        if(strcmp(flight.szFlightId, pFound->flight.szFlightId) == 0){
		printf("%s already exists\n", flight.szFlightId);
            	return NULL;
	}
        if(strcmp(flight.szFlightId, pFound->flight.szFlightId) < 0){	   
            pFound = NULL;}
	
	if(pPrecedes -> pNext == NULL){
		printf("test1\n");
		break;
	}
        pPrecedes = pFound;
	
	printf("test2\n");    
    } 
    //pFound = searchLL(*ppHead, flight.szFlightId);
    pNew = allocateNode(flight);
    if(pPrecedes == NULL)       //at front of list
    {   
        pNew->pNext = *ppHead;
        *ppHead = pNew;
    }
    else if(pPrecedes -> pNext != NULL)
    {
	pNew->pNext = pPrecedes -> pNext;
        pPrecedes->pNext = pNew;
    }else{printf("Test\n");
	pNew->pNext = NULL;
	pPrecedes->pNext = pNew;
    } 
    
    return pNew;*/



 

Flight findFlight(int count, char* flightId, int reqSeats, Flight flightM[]){
   int i;
   Flight err;
   //printf("***%s***\n", flightId);
   for(i=0;i<count;i++){
      if(strcmp(flightId, flightM[i].szFlightId) == 0){
         if(flightM[i].iAvailSeatCnt - reqSeats >= 0){
            flightM[i].iAvailSeatCnt -= reqSeats;
            return flightM[i];
         }
         else{

          err.iAvailSeatCnt = -2;
          return err;
         }
      }
   }
   err.iAvailSeatCnt = -1;
   return err;
}




/******************** sortFlights **************************************
    void sortFlights(Flight flightM[], int iFlightCnt)
Purpose:

Parameters:
  
Returns:
    
Notes:
    
**************************************************************************/
void sortFlights(Flight flightM[], int iFlightCnt)
{
   Flight t;
   int i, s = 1;
    while (s) {
        s = 0;
        for (i = 1; i < iFlightCnt; i++) {
            if (strcmp(flightM[i].szFlightId, flightM[i-1].szFlightId) <  0){
                t = flightM[i];
                flightM[i] = flightM[i - 1];
                flightM[i - 1] = t;
                s = 1;
            }
        }
    }
}

/********************processCustomerCommand *****************************
    void processCustomerCommand(Flight flightM[], int iFlightCnt
     , char *pszSubCommand, char *pszRemainingInput
     , Customer *pCustomer, double *pdCustomerRequestTotalCost )
Purpose:
    Processes the subcommands associated with the CUSTOMER command:
                 CUSTOMER BEGIN cGender   szBirthDt   szEmailAddr    szFullName
                     specifies the beginning of customer request and includes 
                     all the identification information from program 2.
                 CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
                     specifies the address for a customer (separated by commas)
                 CUSTOMER REQUEST szFlightId iNumSeats
                     specifies a single flight request.  Steps:
                     >  Print the flight ID and requested number of seats
                     >  Lookup the flight ID using a binary search.  If 
                        not found, print a warning (but do not terminate your 
                        program) and return.
                     >  If found, try to satisfy the entire requested number 
                        of seats.  If not enough seats, print a warning and 
                        return.
                     >  Print the unit price and cost.
                     >  Accumulate the total cost for this customer

                 CUSTOMER COMPLETE
                     specifies the completion of the list of flight requests 
                     for a customer.
Parameters:
    I/O Flight flightM[]              Array of flights
    I   int   iFlightCnt              Number of elments in flightM[]
    I   char  *pszSubCommand          Should be BEGIN, ADDRESS, REQUEST or
COMPLETE
    I   char  *pzRemainingInput       Points to the remaining characters in the
input
                                      line (i.e., the characters that following
the
                                      subcommand).
    I/O Customer *pCustomer           The BEGIN subcommand begins a new
customer.  The 
                                      customer's Request Total Cost must be set
to 0.
    I/O double   *pdCustomerRequestTotalCost     The customer total cost.  This
changes depending
                                      on the subcommand:
                                          BEGIN - set to 0
                                          REQUEST - add the cost  (unless there
is a warning) 
Notes:

**************************************************************************/
void processCustomerCommand(Node **ppHead
    , char *pszSubCommand, char *pszRemainingInput
    , Customer *pCustomer, double *pdCustomerRequestTotalCost)
{
    int iScanfCnt;
    FlightRequest flightRequest;

    // Determine what to do based on the subCommand
    if (strcmp(pszSubCommand, "BEGIN") == 0)
    {
                iScanfCnt = sscanf(pszRemainingInput, "%c %10s %40s %30[^\n]\n" 
             , &pCustomer->cGender
             , pCustomer->szBirthDt
             , pCustomer->szEmailAddr
             , pCustomer->szFullName);
        
        // Check for bad customer identification data
        
        // get the Customer Identification Information
        // your code

        if (iScanfCnt < 4)
            exitError(ERR_CUSTOMER_ID_DATA, pszRemainingInput);

    }
    else if (strcmp(pszSubCommand, "COMPLETE") == 0)
    {
        // print the customer's total cost
        printf("\t\t\t\t\tTotal Cost\t\t%.2lf\n", *pdCustomerRequestTotalCost);
        *pdCustomerRequestTotalCost  = 0;
        printf("********************************************************************\n");


    }
    else if (strcmp(pszSubCommand, "ADDRESS") == 0)
    {
        //read an address record


        iScanfCnt = sscanf(pszRemainingInput, "%30[^,], %20[^,], %2[^,], %5[^\n]\n" 
            , pCustomer->szStreetAddress
            , pCustomer->szCity
            , pCustomer->szStateCd
            , pCustomer->szZipCd);

        // Check for bad customer address data
        if (iScanfCnt < 4)
            exitError(ERR_CUSTOMER_ADDRESS_DATA, pszRemainingInput);
        
        printf("******************** Flight Reservation Request ********************\n");
        
        printf("%s %s (%c %s)\n" 
            , pCustomer->szEmailAddr
            , pCustomer->szFullName
            , pCustomer->cGender
            , pCustomer->szBirthDt);


        printf("%s\n%s %s %s\n"
            , pCustomer->szStreetAddress
            , pCustomer->szCity
            , pCustomer->szStateCd
            , pCustomer->szZipCd);


        // get the postal address 
        // your code 

        printf("\t\t\t\t%-10s %8s %10s %8s\n"
            , "Flight Id"
            , "Quantity"
            , "Unit Price"
            , "Cost");
    }
    else if (strcmp(pszSubCommand, "REQUEST") == 0)
    {
        Node *n;
        // get a flight request
        // There are many flight requests for a customer.
        // We expect to receive a szFlightId equal to "END"
        // marking the end of the flight requests.
        // Read flight requests until EOF. 

            iScanfCnt = sscanf(pszRemainingInput, "%10s %4d"
                , flightRequest.szFlightId
                , &flightRequest.iRequestSeats);

            // Check for bad reservation data
            if (iScanfCnt < 2)
                exitError(ERR_RESERVATION_DATA, pszRemainingInput);

            // If we received an END in flight, we have reached
            // the end of the requests for this customer
 
            n = searchLL(*ppHead , flightRequest.szFlightId);
                       
            //printf("CUSTOMER REQUEST %-10s\t%5d\n", flightRequest.szFlightId
            //    , flightRequest.iRequestSeats);
             
            if(n == NULL){
                  printf("\t\t\t\t%-10s %8d %10s %8s\n", flightRequest.szFlightId,
                     flightRequest.iRequestSeats, "*** flight ID not found","");
            }
            else if(flightRequest.iRequestSeats > n->flight.iAvailSeatCnt){
                  printf("\t\t\t\t%-10s %8d %10s %8s\n", flightRequest.szFlightId,
                     flightRequest.iRequestSeats, "*** not enough available seats to satisfy request","");
            }
            else{
                  *pdCustomerRequestTotalCost += n-> flight.dSeatPrice * flightRequest.iRequestSeats;
                  printf("\t\t\t\t%-10s %8d %10.2lf %8.2lf\n", flightRequest.szFlightId,
                     flightRequest.iRequestSeats, n-> flight.dSeatPrice, 
                     n-> flight.dSeatPrice * flightRequest.iRequestSeats);
		  n-> flight.iAvailSeatCnt -= flightRequest.iRequestSeats;
                  

            }                   

       

  
    }
    else printf("   *** %s %s\n", ERR_CUSTOMER_SUB_COMMAND, pszSubCommand);
}
/********************processFlightCommand *****************************
    void processFlightCommand(Flight flightM[], int iFlightCnt
         , char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the FLIGHT command:
        FLIGHT INCREASE szFlightId iQuantity
               increase the available seats for a flight by the specified
quantity.
        FLIGHT SHOW szFlightId    
               requests a display of a particular flight.  Show all of its
information.
Parameters:

w
p
r

w
p
r
    I/O Flight flightM[]              Array of flights
    I   int   iFlightCnt              Number of elments in flightM[]
    I   char  *pszSubCommand          Should be RECEIVE or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the
input
                                      line (i.e., the characters that following
the
                                      subcommand).
Notes:

**************************************************************************/
void processFlightCommand(Node **ppHead
                             , char *pszSubCommand, char *pszRemainingInput)
{
    Flight flight;
    int iQuantity;      // quantity of seats 
    int iScanfCnt;
    Node *n;
    FlightRequest flightRequest;
    

    
       
    if (strcmp(pszSubCommand, "INCREASE") == 0)
    {
            iScanfCnt = sscanf(pszRemainingInput, "%10s %4d"
                , flightRequest.szFlightId
                , &flightRequest.iRequestSeats);

            // Check for bad reservation data
            if (iScanfCnt < 2)
                  exitError(ERR_FLIGHT_SUB_COMMAND, pszRemainingInput);

        n = searchLL(*ppHead,flightRequest.szFlightId);
                       
        //printf("FLIGHT INCREASE %-10s %d\n", flightRequest.szFlightId, flightRequest.iRequestSeats);

        if(n == NULL){
               printf("\t\t\t\t%10s\n", "*** flight ID not found");
        }
        else{
               n -> flight.iAvailSeatCnt += flightRequest.iRequestSeats;
        }

    }else if (strcmp(pszSubCommand, "SHOW") == 0)
    {    
            iScanfCnt = sscanf(pszRemainingInput, "%10s"
                , flightRequest.szFlightId);

            // Check for bad reservation data
            if (iScanfCnt < 1)
            exitError(ERR_FLIGHT_SUB_COMMAND, pszRemainingInput);
            n = searchLL(*ppHead, flightRequest.szFlightId);
                       
            //printf("FLIGHT SHOW %-10s\n", flightRequest.szFlightId);

            if(n == NULL){
                  printf("\t\t\t\t%10s\n", "*** flight ID not found");
            }
            else{
                  printf("\t%s\t%s\t%s\t%s\t%d\t%lf\n",  n -> flight.szFlightId,
                                                         n -> flight.szFrom, 
                                                         n -> flight.szDest, 
                                                         n -> flight.szDepartTm,
                                                         n -> flight.iAvailSeatCnt,
                                                         n -> flight.dSeatPrice);

            }           

    }else if(strcmp(pszSubCommand, "NEW") == 0){
	iScanfCnt = sscanf(pszRemainingInput, "%s %s %s %s %i %lf\n",
		flight.szFlightId,
                flight.szFrom,
                flight.szDest,
                flight.szDepartTm,
                &flight.iAvailSeatCnt,
                &flight.dSeatPrice);
	if(iScanfCnt < 6)
        exitError(ERR_FLIGHT_DEF_DATA, pszRemainingInput);
	insertLL(ppHead, flight);
		
    }
		
	
   


}
/******************** search *****************************
    int search(Flight flightM[], int iFlightCnt, char *pszMatchFlightId)
Purpose:
    
Parameters:
    I   Flight flightM[]               Array of flights
    I   int   iFlightCnt               Number of elments in flightM[]
    I   char *pszMatchFlightId         Flight Id to find in the array
Returns:
    >= 0   subscript of where the match value was found
    -1     not found
Notes:

**************************************************************************/



































