#include <stdlib.h>
#include <stdio.h>
#include "covid.h"

RegistrationList rl_create()
{
	RegistrationList list = malloc(sizeof(struct registrationlist));
	list->array = calloc(1, sizeof(struct registration));
	list->count = 0;
	list->size = 1;
	return list;
}

void rl_add(RegistrationList list, Registration item)
{
	if (list->count >= list->size) {		
		list->size = list->size * 2 + 1;
		list->array = realloc(list->array, list->size * sizeof(Registration));	
		printf("|", list->size);
	}
	list->array[list->count++] = item;
}
