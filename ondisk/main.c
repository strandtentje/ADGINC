#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
		tmpReg = r_create();

		scanresult = fscanf(file, "%d %d %d %s %s %d %d %d", 
			&tmpReg->timestamp->year, &tmpReg->timestamp->month, &tmpReg->timestamp->day, 
			tmpCity, tmpState, &tmpReg->locationcode, 
			&tmpReg->statistic->cases, &tmpReg->statistic->deaths);

		tmpReg->city =  StringIndex_public_StringIndex_navigateTo(cities, tmpCity);
		tmpReg->state = StringIndex_public_StringIndex_navigateTo(states, tmpState);

		if (StringIndex_public_string_setText(tmpReg->city, tmpCity) == tmpCity) {
			tmpCity = calloc(40, sizeof(char));
		}
		if (StringIndex_public_string_setText(tmpReg->state, tmpState) == tmpState) {
			tmpState = calloc(40, sizeof(char));
		}

		StringIndex_public_addReference(tmpReg->city, list->count);
		StringIndex_public_addReference(tmpReg->state, list->count);

		rl_add(list, tmpReg);
	}
	while(tmpReg->timestamp->year > 0);	
}



int main(int argc, char** argv) 
{	
	RegistrationList list = rl_create();
	StringIndex cities = StringIndex_public_static_StringIndex_constructor();
	StringIndex states = StringIndex_public_static_StringIndex_constructor();

	printf("!\n");

	char *param = calloc(40, sizeof(char));
	StringIndex result;
	Registration item;

	clock_t start, end;
	double cpu_time_used;

	for (char cmd = 'h'; cmd != 'x'; scanf("%c", &cmd))
	{
		switch(cmd) {
		case 'h':
			printf("%d records are loaded\n", list->count);
			printf("c for results by city\n");
			printf("s for results by state\n");
			printf("l to load file\n");
			printf("x to stop\n");
			printf("h help for help\n");
			break;
		case 'l':
			printf("Relative file path?\n");
			scanf("%s", param);
			FILE *file = fopen(param, "r");
			loadfromfile(file, list, cities, states);
			fclose(file);
			printf("\n%d records are loaded\n", list->count);
			break;
		case 'c':
			printf("Query?\n");
			scanf("%s", param);
			printf("Navigating.\n");
			start = clock();
			result = StringIndex_public_StringIndex_navigateTo(cities, param);
			printf("Amount of results: %d\n", result->referencesCount);
			for (int i = 0; i < result->referencesCount; i++) {
				item = list->array[result->references[i]];
				printf("%s %s %d\n", item->state->text, item->city->text, item->statistic->cases);
			}
			end = clock();			
			printf("start ticks end ticks %d - %d / %d\n", start, end, CLOCKS_PER_SEC);
			break;
		case 's':
			printf("Query?\n");
			scanf("%s", param);
			printf("Navigating.\n");
			result = StringIndex_public_StringIndex_navigateTo(states, param);
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