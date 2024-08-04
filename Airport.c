#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Airport.h"
#include "General.h"

int	isSameAirport(const Airport* pPort1, const Airport* pPort2)
{
	if (!pPort1 || !pPort2)
		return 0;
	if (strcmp(pPort1->code, pPort2->code) == 0)
		return 1;

	return 0;
}

int	isAirportCode(const Airport* pPort1, const char* code)
{
	if (!pPort1)
		return 0;
	if (strcmp(pPort1->code, code) == 0)
		return 1;

	return 0;
}

int	initAirportNoCode(Airport* pPort)
{
	if (!getAirportName(pPort))
		return 0;
	pPort->country = getStrExactName("Enter airport country: ");
	if (!pPort->country)
		return 0;
	return 1;
}

void printAirport(const Airport* pPort)
{
	printf("Airport name: %-20s\t", pPort->name);
	printf("Country: %-10s\tCode: %s\n", pPort->country, pPort->code);
}

void changeName(char* name, int count, char** wordsArray)
{
	for (int i = 0; i < count; i++)
	{
		strcat(name, wordsArray[i]);
		if (i != count - 1) //not last
		{
			strcat(name, " ");
			if (count % 2 == 0)
				strcat(name, " ");
		}
	}
}

int	getAirportName(Airport* pPort)
{
	char temp[255];
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;

	while (count == 0)
	{
		printf("Enter airport name: ");
		myGets(temp, MAX_STR_LEN);
		wordsArray = splitCharsToWords(temp, &count, &totalLength);
	}
	pPort->name = allocateRightLength(wordsArray,count, totalLength);
	if (!pPort->name)
		return 0;

	changeName(pPort->name,count, wordsArray);
		
	//clean temp data
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);
	return 1;
}

char* allocateRightLength(char** wordsArray,int count, int totalLength)
{
	if (count == 1)
		totalLength += (int)strlen(wordsArray[0]);
	else {
		if (count % 2 == 0)
			totalLength += (count - 1) * 2 + 1;
		else
			totalLength += count;
	}

	char* name = (char*)calloc(totalLength, sizeof(char));
	if (!name)
	{
		for (int i = 0; i < count; i++)
			free(wordsArray[i]);
		free(wordsArray);
		return NULL;
	}

	return name;
}

void getAirportCode(char* code)
{
	char temp[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("enter airport code (%d UPPER CASE letters): ", IATA_LENGTH);
		myGets(temp, MAX_STR_LEN);
		if (strlen(temp) != IATA_LENGTH)
		{
			printf("Code need to be %d letters\n", IATA_LENGTH);
			ok = 0;
		}
		else {
			for (int i = 0; i < IATA_LENGTH; i++)
			{
				if (temp[i] < 'A' || temp[i] > 'Z')
				{
					printf("Code need to be upper case letters\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);

	strcpy(code, temp);
}

void freeAirport(Airport* pPort)
{
	free(pPort->name);
	free(pPort->country);
	free(pPort);
}

