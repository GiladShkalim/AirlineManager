#include <stdlib.h>
#include "PassengersList.h"

int addPassenger(PassengersList* list, Person person) {
	PassengerNode* node = (PassengerNode*)malloc(sizeof(PassengerNode));
	if (!node) {
		return 0;
	}

	node->person = person;
	node->next = NULL;

	if (list->head == NULL) {
		node->prev = NULL;
		list->head = node;
	}
	else {
		PassengerNode* current = list->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node;
		node->prev = current;
	}

	return 1;
}

void printPassengers(PassengersList list)
{
	PassengerNode* current = list.head;
	while (current != NULL) {
		printf("                  ");
		printPerson(current->person);
		printf("\n");
		current = current->next;
	}
}
