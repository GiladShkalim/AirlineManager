#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"

int	initManager(AirportManager* pManager, const char* fileName)
{
	char buffer[MAX_STR_LEN];
	FILE* pFile;
	int numOfAirports;
	pFile = fopen(fileName,"r");
	if (pFile)
	{
		if (L_init(&(pManager->airportsArray)))
		{

			if (fgets(buffer, MAX_STR_LEN, pFile))
			{
				numOfAirports = atoi(buffer);
				for (int i = 0; i < numOfAirports; i++)
				{
					Airport* newAirport = (Airport*)malloc(sizeof(Airport));
					if (!newAirport)
						return 0;
					L_insert(&(pManager->airportsArray), newAirport);
					fgetc(pFile);
					if (!loadAirportFromFile(&pManager->airportsArray, pFile))
					{
						printf("Error loading airport from file\n");
						fclose(pFile);
						return 0;
					}
				}
				fclose(pFile);
				return 1;
			}
		}
	}
	if (L_init(&(pManager->airportsArray)))
	{
		fclose(pFile);
		return 2;
	}

	return 0;
}

int		loadAirportFromFile(Airport* pAir, FILE* pFile)
{
	char temp[MAX_STR_LEN];
	if (!pAir)
		return 0;

	myGetsFile(temp, MAX_STR_LEN, pFile);
	pAir->name = getDynStr(temp);

	myGetsFile(temp, MAX_STR_LEN, pFile);
	pAir->country = getDynStr(temp);

	myGetsFile(temp, MAX_STR_LEN, pFile);
	pAir->code[IATA_LENGTH + 1] = getDynStr(temp);
	return 1;

}

int saveManagerToFile(const AirportManager* pManager, const char* fileName) {
	int count = 0;

	FILE* file = fopen(fileName, "w");
    if (!file)
		return 0;
	
	NODE* current = pManager->airportsArray.head.next;
	
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
    newAirport->name = strdup(airport->name);
    newAirport->country = strdup(airport->country);
    strncpy(newAirport->code, airport->code, IATA_LENGTH);
    newAirport->code[IATA_LENGTH] = '\0';

    newNode->key = newAirport;

    newNode->next = pManager->airportsArray.head.next;
    pManager->airportsArray.head.next = newNode;
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

    NODE* current = &(pManager->airportsArray.head);
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;

    return 1; // Success
}

int  initAirport(Airport* pPort, AirportManager* pManager)
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

Airport* findAirportByCode(const AirportManager* pManager, const char* code) {
    NODE* current = pManager->airportsArray.head.next;
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
    NODE* current = pManager->airportsArray.head.next;
    while (current != NULL) {
        count++;
        printAirport((Airport*)current->key);
        printf("\n");
        current = current->next;
    }
    printf("There are %d airports\n", count);
}

void	freeManager(AirportManager* pManager)
{
	freeAirportArr(pManager);
}


void freeAirportArr(AirportManager* pManager) {
    NODE* current = pManager->airportsArray.head.next;
    while (current != NULL) {
        NODE* toFree = current;
        freeAirport((Airport*)current->key);
        free(current->key);
        current = current->next;
        free(toFree);
    }
    pManager->airportsArray.head.next = NULL;
}