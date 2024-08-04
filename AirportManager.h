#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "list.h"

typedef struct
{
	LIST airports;
	LIST airlines;
} AirportManager;

int     initManager(AirportManager* pManager);
int     initManagerFromTextFile(AirportManager* pManager, const char* fileName);
int     initManagerFromBinFile(AirportManager* pManager, const char* fileName);
int		loadAirportFromFile(Airport* pAir, FILE* pFile);
int     saveManagerToTextFile(AirportManager* pManager, const char* fileName);
int     saveManagerToBinFile(AirportManager* pManager, const char* fileName);
int		addAirport(AirportManager* pManager);
int		addAirline(AirportManager* pManager);
int addPlaneToManager(AirportManager* pManager);
int addFlightToManager(AirportManager* pManager);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
int		checkUniqeCode(const char* code, const AirportManager* pManager);
int    managerSortAirlineFlights(AirportManager* pManager);
void	printAirports(AirportManager* pManager);
void	printAirlines(AirportManager* pManager);
void	freeManager(AirportManager* pManager);

#endif
