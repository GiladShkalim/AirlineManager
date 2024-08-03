#include <stdio.h>
#include <stdlib.h>
#include "Airline.h"
#include "AirportManager.h"
#include "General.h"

#define MANAGER_FILE_NAME_SRC "airport_authority.txt"
#define AIRLINE_FILE_NAME_SRC "airline.bin"

#define MANAGER_FILE_NAME_DST "airport_authority_out.txt"
#define AIRLINE_FILE_NAME_DST "airline_out.bin"

#define CHECK(res) if (!res) printf("\n      ERROR\n")

int initManagerAndAirline(AirportManager* pManager, Airline* pCompany);

int main()
{
	AirportManager manager;
	initManager(&manager);
	Airline company;

	int opt = 1, res;

	while (opt != 0) {
		printf("\n-------------------------------------\n\n");
		printf("Please choose one of the following options:\n");
		printf("  0 - Exit\n");
		printf("  1 - Init new airport manager\n");
		printf("  2 - Load airport manager from text file\n");
		printf("  3 - Print airports\n");
		printf("  4 - Print airlines\n");
		printf("  5 - Add airport\n");
		printf("  6 - Add airline\n");
		printf("  7 - Add plane to airline\n");

		/*printf("  2 - Print Airline\n");
		printf("  4 - Add Flight Save\n");
		printf("  5 - Add Plane Save\n");
		printf("  6 - Sort\n");
		printf("  7 - Find\n");*/


		scanf("%d", &opt);

		switch (opt)
		{
		case 0:
			break;

		case 1:
			freeManager(&manager);
			CHECK(initManager(&manager));
			break;

		case 2:
			freeManager(&manager);
			CHECK(initManagerFromTextFile(&manager, MANAGER_FILE_NAME_SRC));
			break;

		case 3:
			printAirports(&manager);
			break;

		case 4:
			printAirlines(&manager);
			break;

		case 5:
			CHECK(addAirport(&manager));
			break;

		case 6:
			CHECK(addAirline(&manager));
			break;

		case 7:
			CHECK(addPlaneToManager(&manager));
			break;

		case 100:
			res = initManagerFromTextFile(&manager, MANAGER_FILE_NAME_SRC);
			if (!res) {
				printf("error init manager\n");
				return 0;
			}
			initAirlineFromFile(&manager, AIRLINE_FILE_NAME_SRC);
			freeManager(&manager);
			break;



		case 400:
			if (!initManagerAndAirline(&manager, &company))
			{
				printf("error init");
				return 0;
			}
			addFlight(&company, &manager);
			printf("\n");
			// printFlightArr(company.flightArr, company.flightCount);
			printf("\n");
			saveAirlineToFile(&company, AIRLINE_FILE_NAME_DST);
			freeAirline(&company);
			initAirlineFromFile(&manager, AIRLINE_FILE_NAME_DST);
			printf("\n");
			// printFlightArr(company.flightArr, company.flightCount);
			printf("\n");
			freeManager(&manager);
			freeAirline(&company);
			break;

		case 500:
			if (!initManagerAndAirline(&manager, &company))
			{
				printf("error init");
				return 0;
			}
			addPlaneToAirline(&company);
			// printPlanesArr(company.planeArr, company.planeCount);
			printf("\n");
			saveAirlineToFile(&company, AIRLINE_FILE_NAME_DST);
			freeAirline(&company);
			freeManager(&manager);
			initManagerFromTextFile(&manager, MANAGER_FILE_NAME_DST);
			initAirlineFromFile(&company, &manager, AIRLINE_FILE_NAME_DST);
			// printPlanesArr(company.planeArr, company.planeCount);
			printf("\n");
			freeManager(&manager);
			freeAirline(&company);
			break;

		case 600:
			if (!initManagerAndAirline(&manager, &company))
			{
				printf("error init");
				return 0;
			}
			sortFlight(&company);
			printf("\n");
			printAirline(&company);
			freeManager(&manager);
			freeAirline(&company);
			break;

		case 700:
			if (!initManagerAndAirline(&manager, &company))
			{
				printf("error init");
				return 0;
			}
			findFlight(&company);
			sortFlight(&company);
			printf("\n");
			printAirline(&company);
			printf("\n");
			findFlight(&company);
			printf("\n");
			freeManager(&manager);
			freeAirline(&company);
			break;
		}
	}

	freeManager(&manager);

	return 0;
}

int initManagerAndAirline(AirportManager* pManager, Airline* pCompany)
{
	int res = initManagerFromTextFile(pManager, MANAGER_FILE_NAME_SRC);
	if (!res)
	{
		printf("error init manager\n");
		return 0;
	}

	if (res == MANAGER_FILE_NAME_DST)
		return initAirlineFromFile(pCompany, pManager, AIRLINE_FILE_NAME_SRC);
	else
		initAirline(pCompany);
	return 1;
}
