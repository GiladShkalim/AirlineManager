#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"

typedef enum {
	eNotSorted, eSourceCode, eDestCode, eDate, eNofSortTypes
} eSortType;

static const char* SortTypeStr[eNofSortTypes] = { "Not Sorted", "Source Code", "Dest Code", "Date" };

typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	int			planeCount;
	Plane*		planeArr;
	eSortType   flightSortType;
} Airline;

void	initAirline(Airline* pComp);
int	    initAirlineFromFile(AirportManager* pManager, const char* fileName);
int     saveAirlineToFile(const Airline* pComp, const char* fileName);
int		addFlight(Airline* pComp,const AirportManager* pManager);
int		addPlaneToAirline(Airline* pComp);
Plane*	FindAPlane(Airline* pComp);
void	printAirline(const Airline* pComp);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void    sortFlight(Airline* pComp);
void    findFlight(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freePlanes(Plane* arr, int size);
void	freeAirline(Airline* pComp);

#endif
