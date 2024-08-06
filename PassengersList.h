#ifndef __PassengersList__
#define __PassengersList__

#include "Person.h"

typedef struct PassengerNode_ {
	Person person;
	struct PassengerNode_* next;
	struct PassengerNode_* prev;
} PassengerNode;

typedef struct {
	PassengerNode* head;
} PassengersList;

int addPassenger(PassengersList* list, Person person);
void printPassengers(PassengersList list);

#endif
