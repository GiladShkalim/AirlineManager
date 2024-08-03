#ifndef __PERSON__
#define __PERSON__

#include "General.h"

typedef struct {
	int id;
	char name[MAX_STR_LEN];
} Person;

Person getPerson();
void printPerson(Person person);

#endif
