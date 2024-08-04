#ifndef __PERSON__
#define __PERSON__

#include "General.h"

typedef struct {
	int id;
	char name[MAX_STR_LEN];
} Person;

Person getPerson(char* message);
void printPerson(Person person);

#endif
