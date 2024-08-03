#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"

void initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name: ");
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

int	addPlaneToAirline(Airline* pComp)
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
	printf("Type the plane serial number: ");
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


void printAirline(const Airline* pComp)
{
	printf("Airline %s:\n", pComp->name);

	printf("      Has %d planes:\n", pComp->planeCount);
	for (int i = 0; i < pComp->planeCount; i++) {
		printf("          ");
		printPlane(&(pComp->planeArr[i]));
	}

	printf("      Has %d flights:\n", pComp->flightCount);
	for (int i = 0; i < pComp->flightCount; i++) {
		printf("          ");
		printFlight(&((*pComp->flightArr)[i]));
	}
}

void doPrintFlightsWithPlaneType(const Airline* pComp)
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
		return;

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

void findFlight(const Airline* pComp)
{
	Flight* flightFound;
	Flight flightToSearch;
	if (!pComp)
		return;

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

void freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
}

void freePlanes(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		freePlane(&arr[i]);
}

void freeAirline(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->planeArr);
	free(pComp->name);
	free(pComp);
}

int initAirlineFromFile(AirportManager* pManager, const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) return 0;

	int totalAirports;
	fscanf(file, "%d", &totalAirports);

	for (int i = 0; i < totalAirports; i++) {
		char nameBuffer[256], countryBuffer[256], codeBuffer[IATA_LENGTH + 1];

		fscanf(file, "%255s %255s %s", nameBuffer, countryBuffer, codeBuffer);

		Airport tempAirport = {
			.name = nameBuffer,
			.country = countryBuffer
		};
		strncpy(tempAirport.code, codeBuffer, IATA_LENGTH);
		tempAirport.code[IATA_LENGTH] = '\0';

		// addAirportToList(pManager, &tempAirport);
	}

	fclose(file);
	return 1;
}

int saveAirlineToFile(const Airline* pComp, const char* fileName) {
	if (pComp == NULL || fileName == NULL) return 0; // Check for NULL pointers

	// Open the file for writing
	FILE* file = fopen(fileName, "w");
	if (!file) return 0; // If opening the file fails, return 0

	// Write airline's name
	fprintf(file, "Airline Name: %s\n", pComp->name);

	// Write flight count and sort type
	fprintf(file, "Flight Count: %d\nSort Type: %d\n", pComp->flightCount, pComp->flightSortType);

	// Write flights information assuming there's a function or way to get flight details in string format
	fprintf(file, "Flights:\n");
	for (int i = 0; i < pComp->flightCount; i++) {
		// Assuming a function to get flight details as string is available or you can directly access flight details
		// Example: fprintf(file, "Flight %d: Details...\n", i + 1);
	}

	// Write plane count
	fprintf(file, "Plane Count: %d\n", pComp->planeCount);

	// Write planes information assuming there's a function or way to get plane details in string format
	fprintf(file, "Planes:\n");
	for (int i = 0; i < pComp->planeCount; i++) {
		// Assuming a function to get plane details as string is available or you can directly access plane details
		// Example: fprintf(file, "Plane %d: Details...\n", i + 1);
	}

	fclose(file); // Close the file after writing

	return 1; // Return 1 to indicate success
}
