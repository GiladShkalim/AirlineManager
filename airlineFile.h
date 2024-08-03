#ifndef __AIRL__
#define __AIRL__



int	    initAirlineFromFile(AirportManager* pManager, const char* fileName);
int     saveAirlineToFile(const Airline* pComp, const char* fileName);

#endif

