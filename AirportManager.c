#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"
#include "Airline.h"

int initManager(AirportManager* pManager)
{
	if (!L_init(&(pManager->airports))) {
		return 0;
	}
	if (!L_init(&(pManager->airlines))) {
		freeManager(pManager);
		return 0;
	}
	return 1;
}

int	initManagerFromTextFile(AirportManager* pManager, const char* fileName)
{
	char buffer[MAX_STR_LEN];
	FILE* pFile;
	int numOfAirports;

	pFile = fopen(fileName,"r");
	if (!pFile) {
		return 0;
	}

	if (!initManager(pManager)) {
		fclose(pFile);
		return 0;
	}

	if (!fgets(buffer, MAX_STR_LEN, pFile)) {
		fclose(pFile);
		freeManager(pManager);
		return 0;
	}

	numOfAirports = atoi(buffer);
	for (int i = 0; i < numOfAirports; i++) {
		Airport* newAirport = (Airport*)malloc(sizeof(Airport));
		if (!newAirport) {
			fclose(pFile);
			freeManager(pManager);
			return 0;
		}
		if (!loadAirportFromFile(newAirport, pFile)) {
			printf("Error loading airport from file\n");
			free(newAirport);
			fclose(pFile);
			freeManager(pManager);
			return 0;
		}
		L_insert(&(pManager->airports), newAirport);
	}

	fclose(pFile);
	return 1;
}

int loadAirportFromFile(Airport* pAir, FILE* pFile)
{
	char temp[MAX_STR_LEN];
	if (!pAir)
		return 0;

	myGetsFile(temp, MAX_STR_LEN, pFile);
	pAir->name = getDynStr(temp);

	myGetsFile(temp, MAX_STR_LEN, pFile);
	pAir->country = getDynStr(temp);

	myGetsFile(temp, MAX_STR_LEN, pFile);
	strcpy(pAir->code, temp);
	return 1;
}

int saveManagerToFile(const AirportManager* pManager, const char* fileName) {
	int count = 0;

	FILE* file = fopen(fileName, "w");
    if (!file)
		return 0;
	
	NODE* current = pManager->airports.head.next;
	
	while (current != NULL) {
		count++;
		Airport* airport = (Airport*)current->key;
        fprintf(file, "%s\n%s\n%s\n", airport->name, airport->country, airport->code);
        current = current->next;
    }
	rewind(file);
    fprintf(file, "%d\n", count);
	fclose(file);
    return 1;
}

void addAirportToList(AirportManager* pManager, Airport* airport) {
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (!newNode) return;

    Airport* newAirport = (Airport*)malloc(sizeof(Airport));
    newAirport->name = _strdup(airport->name);
    newAirport->country = _strdup(airport->country);
    strncpy(newAirport->code, airport->code, IATA_LENGTH);
    newAirport->code[IATA_LENGTH] = '\0';

    newNode->key = newAirport;

    newNode->next = pManager->airports.head.next;
    pManager->airports.head.next = newNode;
}

int initAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager))
			break;
		printf("This code already in use - enter a different code\n");
	}

	return initAirportNoCode(pPort);
}

int addAirport(AirportManager* pManager) {
    Airport* pPort = (Airport*)calloc(1, sizeof(Airport));
    if (!pPort) {
        return 0;
    }

    if (!initAirport(pPort, pManager)) {
        free(pPort);
        return 0;
    }

    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (!newNode) {
        free(pPort);
        return 0;
    }

    newNode->key = pPort;
    newNode->next = NULL;

    NODE* current = &(pManager->airports.head);
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;

    return 1; // Success
}

int addAirline(AirportManager* pManager)
{
	Airline* airline = (Airline *)malloc(sizeof(Airline));
	if (!airline) {
		return 0;
	}

	initAirline(airline);

	NODE* current = pManager->airlines.head.next;
	while (current != NULL) {
		if (strcmp(((Airline*)(current->key))->name, airline->name) == 0) {
			printf("Airline %s already exists.\n", airline->name);
			freeAirline(airline);
			return 0;
		}
		current = current->next;
	}

	L_insert(&(pManager->airlines), airline);

	return 1;
}

int addPlaneToManager(AirportManager* pManager)
{
	printf("Enter the name of the airline to add the plane to: ");
	char name[MAX_STR_LEN];
	myGets(name, MAX_STR_LEN);

	NODE* current = pManager->airlines.head.next;
	Airline* airline = NULL;
	while (current != NULL) {
		if (strcmp(((Airline*)(current->key))->name, name) == 0) {
			airline = (Airline*)(current->key);
			break;
		}
		current = current->next;
	}

	if (!airline) {
		printf("Airline %s does not exists.\n", name);
		return 0;
	}

	return addPlaneToAirline(airline);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code) {
    NODE* current = pManager->airports.head.next;
    while (current != NULL) {
        Airport* airport = (Airport*)current->key;
        if (isAirportCode(airport, code)) {
            return airport;
        }
        current = current->next;
    }
    return NULL;
}

int checkUniqeCode(const char* code,const AirportManager* pManager)
{
	Airport* port = findAirportByCode(pManager, code);

	if (port != NULL)
		return 0;

	return 1;
}

void printAirports(const AirportManager* pManager) {
    int count = 0;
    NODE* current = pManager->airports.head.next;
	printf("Airports:\n");
    while (current != NULL) {
        count++;
		printf("\t%2d. ", count);
        printAirport((Airport*)current->key);
        current = current->next;
    }
    printf("There are %d airports.\n", count);
}

void printAirlines(const AirportManager* pManager)
{
	int count = 0;
	NODE* current = pManager->airlines.head.next;
	printf("Airlines:\n");
	while (current != NULL) {
		count++;
		printf("  %2d. ", count);
		printAirline((Airline*)current->key);
		current = current->next;
	}
	printf("There are %d airlines.\n", count);

}

void freeManager(AirportManager* pManager)
{
    NODE* current = pManager->airports.head.next;
    while (current != NULL) {
        NODE* toFree = current;
        freeAirport((Airport*)current->key);
        free(current->key);
        current = current->next;
        free(toFree);
    }
    pManager->airports.head.next = NULL;
}
