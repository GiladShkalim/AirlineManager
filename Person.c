#include <stdlib.h>
#include "Person.h"

Person getPerson(char* message)
{
	Person person;

	printf(message);
	myGets(person.name, MAX_STR_LEN);

	person.id = rand() % 100000;

	return person;
}

void printPerson(Person person)
{
	printf("Name - %s, ID - %d", person.name, person.id);
}
