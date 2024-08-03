#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "AirportManager.h"
#include "General.h"




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

		addAirportToList(pManager, &tempAirport);
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