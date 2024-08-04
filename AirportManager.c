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
	int num;

	pFile = fopen(fileName,"r");
	if (!pFile) {
		return 0;
	}

	if (!initManager(pManager)) {
		fclose(pFile);
		return 0;
	}

	// load airports
	if (!fgets(buffer, MAX_STR_LEN, pFile)) {
		fclose(pFile);
		freeManager(pManager);
		return 0;
	}
	num = atoi(buffer);
	for (int i = 0; i < num; i++) {
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

	if (!fgets(buffer, MAX_STR_LEN, pFile)) {
		fclose(pFile);
		freeManager(pManager);
		return 0;
	}

	// load airlines
	if (!fgets(buffer, MAX_STR_LEN, pFile)) {
		fclose(pFile);
		freeManager(pManager);
		return 0;
	}
	num = atoi(buffer);
	for (int i = 0; i < num; i++) {
		Airline* newAirline = (Airline*)malloc(sizeof(Airline));
		if (!newAirline) {
			fclose(pFile);
			freeManager(pManager);
			return 0;
		}
		fgets(buffer, MAX_STR_LEN, pFile);
		buffer[strlen(buffer) - 1] = '\0';
		newAirline->name = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		if (!newAirline->name) {
			fclose(pFile);
			free(newAirline);
			freeManager(pManager);
			return 0;
		}
		strcpy(newAirline->name, buffer);
		newAirline->flightArr = NULL;
		newAirline->flightCount = 0;
		newAirline->planeArr = NULL;
		newAirline->planeCount = 0;
		L_insert(&(pManager->airlines), newAirline);
	}

	fclose(pFile);
	return 1;
}

int initManagerFromBinFile(AirportManager* pManager, const char* fileName)
{
	char buffer[MAX_STR_LEN];
	FILE* pFile;
	int num;

	pFile = fopen(fileName, "r");
	if (!pFile) {
		return 0;
	}

	if (!initManager(pManager)) {
		fclose(pFile);
		return 0;
	}

	// load airports
	fread(&num, sizeof(int), 1, pFile);
	for (int i = 0; i < num; i++) {
		Airport* newAirport = (Airport*)malloc(sizeof(Airport));
		if (!newAirport) {
			fclose(pFile);
			freeManager(pManager);
			return 0;
		}
		getStringFromBinaryFile(buffer, pFile);
		newAirport->name = getDynStr(buffer);
		getStringFromBinaryFile(buffer, pFile);
		newAirport->country = getDynStr(buffer);
		getStringFromBinaryFile(newAirport->code, pFile);
		L_insert(&(pManager->airports), newAirport);
	}

	// load airlines
	fread(&num, sizeof(int), 1, pFile);
	for (int i = 0; i < num; i++) {
		Airline* newAirline = (Airline*)malloc(sizeof(Airline));
		if (!newAirline) {
			fclose(pFile);
			freeManager(pManager);
			return 0;
		}
		getStringFromBinaryFile(buffer, pFile);
		newAirline->name = getDynStr(buffer);
		newAirline->flightArr = NULL;
		newAirline->flightCount = 0;
		newAirline->planeArr = NULL;
		newAirline->planeCount = 0;
		L_insert(&(pManager->airlines), newAirline);
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

int saveManagerToTextFile(AirportManager* pManager, const char* fileName) {
	int count = 0;

	FILE* file = fopen(fileName, "w");
    if (!file)
		return 0;
	
    fprintf(file, "%d\n", L_count(&(pManager->airports)));
	NODE* current = pManager->airports.head.next;
	while (current != NULL) {
		count++;
		Airport* airport = (Airport*)current->key;
        fprintf(file, "%s\n%s\n%s\n", airport->name, airport->country, airport->code);
        current = current->next;
    }

	fprintf(file, "\n%d\n", L_count(&(pManager->airlines)));
	current = pManager->airlines.head.next;
	while (current != NULL) {
		count++;
		Airline* airline = (Airline*)current->key;
		fprintf(file, "%s\n", airline->name);
		current = current->next;
	}

	fclose(file);
    return 1;
}

int saveManagerToBinFile(AirportManager* pManager, const char* fileName)
{
	int count = 0;

	FILE* file = fopen(fileName, "w");
	if (!file)
		return 0;

	count = L_count(&(pManager->airports));
	fwrite(&count, sizeof(int), 1 ,file);
	NODE* current = pManager->airports.head.next;
	while (current != NULL) {
		count++;
		Airport* airport = (Airport*)current->key;
		fwrite(airport->name, strlen(airport->name) + 1, 1, file);
		fwrite(airport->country, strlen(airport->country) + 1, 1, file);
		fwrite(airport->code, strlen(airport->code) + 1, 1, file);
		current = current->next;
	}

	count = L_count(&(pManager->airlines));
	fwrite(&count, sizeof(int), 1, file);
	current = pManager->airlines.head.next;
	while (current != NULL) {
		count++;
		Airline* airline = (Airline*)current->key;
		fwrite(airline->name, strlen(airline->name) + 1, 1, file);
		current = current->next;
	}

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
		if (!findAirportByCode(pManager, pPort->code))
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

int addFlightToManager(AirportManager* pManager)
{
	printf("Enter the name of the airline to add the flight to: ");
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

	return addFlightToAirline(airline, pManager);
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

int managerSortAirlineFlights(AirportManager* pManager)
{
	airlineSortAirlineFlights();
}

void printAirports(AirportManager* pManager) {
    int count = 0;
    NODE* current = pManager->airports.head.next;
	printf("Airports:\n");
    printf("There are %d airports:\n", L_count(&(pManager->airports)));
    while (current != NULL) {
		count++;
		printf("    %2d. ", count);
        printAirport((Airport*)current->key);
        current = current->next;
    }
}

void printAirlines(AirportManager* pManager)
{
	int count = 0;
	NODE* current = pManager->airlines.head.next;
	printf("There are %d airlines:\n", L_count(&(pManager->airlines)));
	while (current != NULL) {
		count++;
		printf("  %2d. ", count);
		printAirline((Airline*)current->key);
		current = current->next;
	}
}

void freeManager(AirportManager* pManager)
{
	if (!pManager) {
		return;
	}

    NODE* current = pManager->airports.head.next;
    while (current != NULL) {
        NODE* toFree = current;
        freeAirport((Airport*)current->key);
        current = current->next;
        free(toFree);
    }
    pManager->airports.head.next = NULL;

	current = pManager->airlines.head.next;
	while (current != NULL) {
		NODE* toFree = current;
		freeAirline((Airline*)current->key);
		current = current->next;
		free(toFree);
	}
	pManager->airlines.head.next = NULL;
}
