#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "General.h"


void	initFlight(Flight* pFlight,Plane* thePlane, const AirportManager* pManager)
{
	pFlight->flightPlane = *thePlane;

	Airport* pPortOr = setAiportToFlight(pManager, "Origin airport - ");
	strcpy(pFlight->sourceCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Destination airport - ");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport - try again:\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);

	getCorrectDate(&pFlight->date);

	pFlight->pilot = getPerson("Enter pilot name: ");
}

int	isFlightFromSourceAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->sourceCode, code) == 0)
		return 1;
		
	return 0;
}


int	isFlightToDestAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->destCode, code) == 0)
		return 1;

	return 0;
}

int	isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type)
{
	return (pFlight->flightPlane.type == type);
}


void printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s",pFlight->sourceCode, pFlight->destCode);
	printf("  |  ");
	printDate(&pFlight->date);
	printf("  |  ");
	printf("Pilot: ");
	printPerson(pFlight->pilot);
	printf("  |  ");
	printPlane(&pFlight->flightPlane);
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s", msg);
		getAirportCode(code);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport with this code - try again:\n");
	} while (port == NULL);

	return port;
}

int compareSourceCode(const void* pFlight1, const void* pFlight2)
{
	return strcmp(((const Flight *)pFlight1)->sourceCode, ((const Flight*)pFlight2)->sourceCode);
}

int compareDestCode(const void* pFlight1, const void* pFlight2)
{
	return strcmp(((const Flight*)pFlight1)->destCode, ((const Flight*)pFlight2)->destCode);
}

int compareDate(const void* pFlight1, const void* pFlight2)
{
	if (((const Flight*)pFlight1)->date.year == ((const Flight*)pFlight2)->date.year)
	{
		if (((const Flight*)pFlight1)->date.month == ((const Flight*)pFlight2)->date.month)
		{
			return ((const Flight*)pFlight1)->date.day - ((const Flight*)pFlight2)->date.day;
		}
		return ((const Flight*)pFlight1)->date.month - ((const Flight*)pFlight2)->date.month;
	}
	return ((const Flight*)pFlight1)->date.year - ((const Flight*)pFlight2)->date.year;
	
}
