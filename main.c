#include <stdio.h>
#include <stdlib.h>
#include "Airline.h"
#include "AirportManager.h"
#include "General.h"

#define MANAGER_TEXT_FILE_NAME_SRC "airport_authority.txt"
#define MANAGER_BIN_FILE_NAME_SRC "airport_authority.bin"

#define MANAGER_TEXT_FILE_NAME_DST "airport_authority_out.txt"
#define MANAGER_BIN_FILE_NAME_DST "airport_authority_out.bin"

#define CHECK(res) if (!res) printf("\n      ERROR\n")

int main()
{
	AirportManager manager;
	initManager(&manager);
	Airline company;

	int opt = 1;

	while (opt != 0) {
		printf("\n-------------------------------------\n\n");
		printf("Please choose one of the following options:\n");
		printf("  0 - Exit\n");
		printf("  1 - Init new airport manager\n");
		printf("  2 - Load airport manager from text file\n");
		printf("  3 - Init airport manager from binary file\n");
		printf("  4 - Save airport manager to text file\n");
		printf("  5 - Save airport manager to binary file\n");
		printf("  6 - Print airports\n");
		printf("  7 - Print airlines\n");
		printf("  8 - Add airport\n");
		printf("  9 - Add airline\n");
		printf(" 10 - Add plane\n");
		printf(" 11 - Add flight\n");
		printf(" 12 - Sort airline flights\n");

		/*printf("  2 - Print Airline\n");
		printf("  4 - Add Flight Save\n");
		printf("  5 - Add Plane Save\n");
		printf("  6 - Sort\n");
		printf("  7 - Find\n");*/


		if (scanf("%d", &opt) != 1) {
			printf("Invalid input.\n");
			break;
		}

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
			CHECK(initManagerFromTextFile(&manager, MANAGER_TEXT_FILE_NAME_SRC));
			break;

		case 3:
			freeManager(&manager);
			CHECK(initManagerFromBinFile(&manager, MANAGER_BIN_FILE_NAME_SRC));
			break;

		case 4:
			CHECK(saveManagerToTextFile(&manager, MANAGER_TEXT_FILE_NAME_SRC));
			break;

		case 5:
			CHECK(saveManagerToBinFile(&manager, MANAGER_BIN_FILE_NAME_SRC));
			break;

		case 6:
			printAirports(&manager);
			break;

		case 7:
			printAirlines(&manager);
			break;

		case 8:
			CHECK(addAirport(&manager));
			break;

		case 9:
			CHECK(addAirline(&manager));
			break;

		case 10:
			CHECK(addPlaneToManager(&manager));
			break;

		case 11:
			CHECK(addFlightToManager(&manager));
			break;

		case 12:
			CHECK(managerSortAirlineFlights(&manager));
			break;

		case 700:
			findFlight(&company);
			airlineSortAirlineFlights(&company);
			printf("\n");
			printAirline(&company);
			printf("\n");
			findFlight(&company);
			printf("\n");
			freeManager(&manager);
			freeAirline(&company);
			break;

		default:
			printf("Invalid option.");
		}
	}

	freeManager(&manager);

	return 0;
}
