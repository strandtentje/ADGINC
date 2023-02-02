#include <stdlib.h>
#include <stdio.h>
#include "stringindex.h"
#include "covid.h"

void loadfromfile(FILE *file, RegistrationList list, StringIndex cities, StringIndex states)
{
	Registration tmpReg;
	char *tmpCity  = calloc(40, sizeof(char));
	char *tmpState = calloc(40, sizeof(char));;

	int scanresult = 0;
	do
	{
		tmpReg = malloc(sizeof(struct registration));
		tmpReg->timestamp   = malloc(sizeof(struct datetime));
		tmpReg->statistic   = malloc(sizeof(struct statistic));

		scanresult = fscanf(file, "%d %d %d %s %s %d %d %d", 
			&tmpReg->timestamp->year, &tmpReg->timestamp->month, &tmpReg->timestamp->day, 
			tmpCity, tmpState, &tmpReg->locationcode, 
			&tmpReg->statistic->cases, &tmpReg->statistic->deaths);

		tmpReg->city =  osi_navigateTo(cities, tmpCity);
		tmpReg->state = osi_navigateTo(states, tmpState);

		if (osi_setText(tmpReg->city, tmpCity) == tmpCity) {
			tmpCity = calloc(40, sizeof(char));
		}
		if (osi_setText(tmpReg->state, tmpState) == tmpState) {
			tmpState = calloc(40, sizeof(char));
		}

		osi_addReference(tmpReg->city, list->count);
		osi_addReference(tmpReg->state, list->count);

		rl_add(list, tmpReg);
	}
	while(tmpReg->timestamp->year > 0);	
}

int main(int argc, char** argv) 
{	
	RegistrationList list = rl_create();
	StringIndex cities = osi_createEmpty();
	StringIndex states = osi_createEmpty();

	FILE *file = fopen("veeldata-2.csv", "r");
	loadfromfile(file, list, cities, states);
	fclose(file);

	printf("!\n");

	char *param = calloc(40, sizeof(char));
	StringIndex result;
	Registration item;

	for (char cmd = 'h'; cmd != 'x'; scanf("%c %s", &cmd, param))
	{
		printf("Understood\n");
		switch(cmd) {
		case 'h':
			printf("%d records are loaded\n", list->count);
			printf("c [name] for results by city\n");
			printf("s [name] for results by state\n");
			printf("h help for help\n");
			break;
		case 'c':
			printf("Navigating.\n");
			result = osi_navigateTo(cities, param);
			printf("Amount of results: %d\n", result->referencesCount);
			for (int i = 0; i < result->referencesCount; i++) {
				item = list->array[result->references[i]];
				printf("%s %s %d\n", item->state->text, item->city->text, item->statistic->cases);
			}
			break;
		case 's':
			printf("Navigating.\n");
			result = osi_navigateTo(states, param);
			printf("Amount of results: %d\n", result->referencesCount);
			for (int i = 0; i < result->referencesCount; i++) {
				item = list->array[result->references[i]];
				printf("%s %s %d\n", item->state->text, item->city->text, item->statistic->cases);
			}
			break;
		default:
			break;
		}	
	}

	return 0;
}