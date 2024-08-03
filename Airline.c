#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"

void	initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->planeArr = NULL;
	pComp->planeCount = 0;
}

int	addFlight(Airline* pComp,const AirportManager* pManager)
{
	if(pComp->planeCount == 0)
	{
		printf("There is no plane in company\n");
		return 0;
	}

	Flight* pFlight = (Flight*)calloc(1, sizeof(Flight));
	if (!pFlight)
		return 0;
	
	Plane* thePlane = FindAPlane(pComp);
	printAirports(pManager);
	initFlight(pFlight, thePlane,pManager);

	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pFlight);
		return 0;
	}
	pComp->flightArr[pComp->flightCount] = pFlight;
	pComp->flightCount++;
	return 1;
}

int		addPlane(Airline* pComp)
{
	pComp->planeArr = (Plane*)realloc(pComp->planeArr, (pComp->planeCount + 1) * sizeof(Plane));
	if (!pComp->planeArr)
		return 0;
	initPlane(&pComp->planeArr[pComp->planeCount], pComp->planeArr, pComp->planeCount);
	pComp->planeCount++;
	return 1;
}

Plane* FindAPlane(Airline* pComp)
{
	printf("Choose a plane from list, type its serial Number\n");
	printPlanesArr(pComp->planeArr,pComp->planeCount);
	int sn;
	Plane* temp = NULL;
	do {
		scanf("%d", &sn);
		temp = findPlaneBySN(pComp->planeArr,pComp->planeCount, sn);
		if (!temp)
			printf("No plane with that serial number! Try again!\n");
	} while (temp == NULL);
	 
	return temp;
}


void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("\n -------- Has %d planes\n", pComp->planeCount);
	printPlanesArr(pComp->planeArr, pComp->planeCount);
	printf("\n\n -------- Has %d flights\n", pComp->flightCount);
	printFlightArr(pComp->flightArr, pComp->flightCount);
}

void	printFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		printFlight(arr[i]);
}

void	printPlanesArr(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		printPlane(&arr[i]);
}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	int count = 0;
	printf("Flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
		{
			printFlight(pComp->flightArr[i]);
			count++;
		}
	}
	if(count == 0)
		printf("Sorry - could not find a flight with plane type %s:\n", GetPlaneTypeStr(type));
	printf("\n");
}

void    sortFlight(Airline* pComp)
{
	int option;
	if (!pComp)
		return NULL;

	printf("\n\n");
	do {
		printf("Base on what field do you want to sort?\n");
		for (int i = 1; i < eNofSortTypes; i++)
			printf("Enter %d for %s\n", i, SortTypeStr[i]);
		scanf("%d", &option);
	} while (option < 1 || option >= eNofSortTypes);
	getchar();
	switch (option)
	{
	case 1: //Source Code
		qsort(pComp->flightArr, pComp->flightCount,sizeof(Flight),compareSourceCode);
		pComp->flightSortType = eSourceCode;
		break;

	case 2: //Dest Code
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight), compareDestCode);
		pComp->flightSortType = eDestCode;
		break;

	case 3:  //Date
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight), compareDate);
		pComp->flightSortType = eDate;
		break;
	}
}

void    findFlight(const Airline* pComp)
{
	Flight* flightFound;
	Flight flightToSearch;
	if (!pComp)
		return NULL;

	switch (pComp->flightSortType)
	{
	case eNotSorted:
		printf("The search cannot be performed, array not sorted\n");
		break;

	case eSourceCode:
		getAirportCode(flightToSearch.sourceCode);
		flightFound = bsearch(&flightToSearch, pComp->flightArr, pComp->flightCount, sizeof(Flight), compareSourceCode);
		if (flightFound)
		{
			printf("Flight found, ");
			printFlight(flightFound);
		}
		else
		{
			printf("Flight was not found\n");
		}
		break;

	case eDestCode: 
		getAirportCode(flightToSearch.destCode);
		flightFound = bsearch(&flightToSearch, pComp->flightArr, pComp->flightCount, sizeof(Flight), compareDestCode);
		if (flightFound)
		{
			printf("Flight found, ");
			printFlight(flightFound);
		}
		else
		{
			printf("Flight was not found\n");
		}
		break;

	case eDate:  
		getCorrectDate(&(flightToSearch.date));
		flightFound = bsearch(&flightToSearch, pComp->flightArr, pComp->flightCount, sizeof(Flight), compareDate);
		if (flightFound)
		{
			printf("Flight found, ");
			printFlight(flightFound);
		}
		else
		{
			printf("Flight was not found\n");
		}
		break;
	}
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
}

void	freePlanes(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		freePlane(&arr[i]);
}

void	freeCompany(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->planeArr);
	free(pComp->name);
}
