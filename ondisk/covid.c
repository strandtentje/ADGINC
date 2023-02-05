#include <stdlib.h>
#include <stdio.h>
#include "covid.h"

TemporaryRegistration tmp_create()
{
	TemporaryRegistration registration = malloc(sizeof(struct temporaryRegistration));
	registration->city  = calloc(40, sizeof(char));
	registration->state = calloc(40, sizeof(char));
	registration->year = calloc(4, sizeof(char));
	registration->month = calloc(2, sizeof(char));
	registration->day = calloc(2, sizeof(char));
	registration->isoDate = calloc(11, sizeof(char));
	registration->hexCases = calloc(11, sizeof(char));
	registration->hexDeaths = calloc(11, sizeof(char));
	registration->cases = 0;
	registration->deaths = 0;
	return registration;
}

char *tmp_deriveIsoDate(TemporaryRegistration this)
{
	snprintf(this->isoDate, 11, "%s-%s-%s", this->year, this->month, this->day);
	return this->isoDate;
}

char *tmp_indexibleCases(TemporaryRegistration this)
{
	snprintf(this->hexCases, 11, "%08x", this->cases);
	return this->hexCases;
}

char *tmp_indexibleDeaths(TemporaryRegistration this)
{
	snprintf(this->hexDeaths, 11, "%08x", this->deaths);	
	return this->hexDeaths;
}

char *tmp_getPermanentCity(void *uncast)
{
	TemporaryRegistration this = (TemporaryRegistration)uncast;
	char *permanent = this->city;
	this->city = calloc(40, sizeof(char));
	return permanent;
}

char *tmp_getPermanentState(void *uncast)
{
	TemporaryRegistration this = (TemporaryRegistration)uncast;
	char *permanent = this->state;
	this->state = calloc(40, sizeof(char));
	return permanent;
}

char *tmp_getPermanentDate(void *uncast)
{
	TemporaryRegistration this = (TemporaryRegistration)uncast;
	char *permanent = this->isoDate;
	this->isoDate = calloc(40, sizeof(char));
	return permanent;
}

char *tmp_getLegibleCases(void *uncast)
{
	TemporaryRegistration this = (TemporaryRegistration)uncast;
	char *legibleCases = calloc(11, sizeof(char));
	snprintf(legibleCases, 11, "%d", this->cases);
	return legibleCases;	
}

char *tmp_getLegibleDeaths(void *uncast)
{
	TemporaryRegistration this = (TemporaryRegistration)uncast;
	char *legibleDeaths = calloc(11, sizeof(char));
	snprintf(legibleDeaths, 11, "%d", this->deaths);
	return legibleDeaths;
}

Registration r_create()
{
	Registration registration = malloc(sizeof(struct registration));	
	return registration;
}

RegistrationList rl_create()
{
	RegistrationList list = malloc(sizeof(struct registrationlist));
	list->array = calloc(1, sizeof(struct registration));
	list->count = 0;
	list->size = 1;
	return list;
}

int rl_add(RegistrationList list, Registration item)
{
	if (list->count >= list->size) {		
		list->size = list->size * 2 + 1;
		list->array = realloc(list->array, list->size * sizeof(Registration));	
		printf("|", list->size);
	}
	item->primaryKey = list->count;
	list->array[list->count++] = item;
	return item->primaryKey;
}
