#include <stdlib.h>
#include "Person.h"

Person getPerson()
{
	Person person;

	printf("Person name: ");
	scanf("%s", person.name);

	person.id = rand() % 100000;

	return person;
}

void printPerson(Person person)
{
	printf("Name: %15s\tID: %8d\n", person.name, person.id);
}
