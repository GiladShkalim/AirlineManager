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
int		loadAirportFromFile(Airport* pAir, FILE* pFile);
int     saveManagerToFile(const AirportManager* pManager, const char* fileName);
int		addAirport(AirportManager* pManager);
// int		initAirport(Airport* pPort, AirportManager* pManager);
int		addAirline(AirportManager* pManager);
int addPlaneToManager(AirportManager* pManager);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
int		checkUniqeCode(const char* code, const AirportManager* pManager);
void	printAirports(const AirportManager* pManager);
void	printAirlines(const AirportManager* pManager);
void	freeManager(AirportManager* pManager);

#endif
