#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "stringindex.h"
#include "covid.h"

void loadfromfile(
	FILE *file, 
	RegistrationList list, 
	StringIndex cities,
	StringIndex states,
	StringIndex dates,
	StringIndex cases,
	StringIndex deaths)
{	
	TemporaryRegistration temporary = tmp_create();

	int scanresult = 0;
	do
	{
		Registration newReg = r_create();
		scanresult = fscanf(file, "%4s %2s %2s %40s %40s %d %d %d", 
			temporary->year, temporary->month, temporary->day,
			temporary->city, temporary->state, &newReg->locationcode, 
			&temporary->cases, &temporary->deaths);

		newReg->city 	= StringIndex_public_StringIndex_navigateTo(cities, temporary->city);
		newReg->state 	= StringIndex_public_StringIndex_navigateTo(states, temporary->state);
		newReg->date 	= StringIndex_public_StringIndex_navigateTo(dates, tmp_deriveIsoDate(temporary));
		newReg->cases 	= StringIndex_public_StringIndex_navigateTo(cases, tmp_indexibleCases(temporary));
		newReg->deaths 	= StringIndex_public_StringIndex_navigateTo(deaths, tmp_indexibleDeaths(temporary));

		StringIndex_public_string_sourceText(newReg->city, tmp_getPermanentCity, temporary);			
		StringIndex_public_string_sourceText(newReg->state, tmp_getPermanentState, temporary);
		StringIndex_public_string_sourceText(newReg->date, tmp_getPermanentDate, temporary);
		StringIndex_public_string_sourceText(newReg->cases, tmp_getLegibleCases, temporary);
		StringIndex_public_string_sourceText(newReg->deaths, tmp_getLegibleDeaths, temporary);


		rl_add(list, newReg);
		StringIndex_public_addReference(newReg->city, newReg->primaryKey);
		StringIndex_public_addReference(newReg->state, newReg->primaryKey);
		StringIndex_public_addReference(newReg->date, newReg->primaryKey);
		StringIndex_public_addReference(newReg->cases, newReg->primaryKey);
		StringIndex_public_addReference(newReg->deaths, newReg->primaryKey);
	}
	while(!feof(file));	
}

int main(int argc, char** argv) 
{	
	RegistrationList list = rl_create();
	StringIndex cities = StringIndex_public_static_StringIndex_constructor(StringIndex_tokenizeOnlyCapitals);
	StringIndex states = StringIndex_public_static_StringIndex_constructor(StringIndex_tokenizeOnlyCapitals);
	StringIndex dates  = StringIndex_public_static_StringIndex_constructor(StringIndex_tokenizeOnlyNumbers);
	StringIndex cases  = StringIndex_public_static_StringIndex_constructor(StringIndex_tokenizeOnlyHex);
	StringIndex deaths = StringIndex_public_static_StringIndex_constructor(StringIndex_tokenizeOnlyHex);

	printf("!\n");

	int iParam1 = 0;
	int iParam2 = 0;
	char *param1 = calloc(40, sizeof(char));
	char *param2 = calloc(40, sizeof(char));
	StringIndex result;
	StringIndex resultend;

	double cpu_time_used;

	void printHead() {		
		printf("| %11s | %40s | %40s | %8s | %8s |\n", "date", "state", "city", "cases", "deaths");
		printf("+%*c+%*c+%*c+%*c+%*c+\n", 13, ' ', 42, ' ', 42, ' ', 10, ' ', 10, ' ');
	}

	void printResult(StringIndex result, void* param1) {
		Registration item;
		for (int i = 0; i < result->referencesCount; i++) {			
			item = list->array[result->references[i]];
			printf("| %11s | %40s | %40s | %8s | %8s |\n", item->date->text, item->state->text, item->city->text, item->cases->text, item->deaths->text);
		}
	}

	for (char cmd = 'h'; cmd != 'x'; scanf("%c", &cmd))
	{
		switch(cmd) {
		case 'h':
			printf("%d records are loaded\n", list->count);
			printf("c for results by city\n");
			printf("s for results by state\n");
			printf("d for results by date\n");
			printf("a for results by cases\n");
			printf("e for results by deaths\n");
			printf("l to load file\n");
			printf("x to stop\n");
			printf("h help for help\n");
			break;
		case 'l':
			printf("Relative file path?\n");
			scanf("%s", param1);
			FILE *file = fopen(param1, "r");
			loadfromfile(file, list, cities, states, dates, cases, deaths);
			fclose(file);
			printf("\n%d records are loaded\n", list->count);
			break;
		case 'a':
			printf("Min cases max cases?\n");
			scanf("%d %d", &iParam1, &iParam2);
			snprintf(param1, 11, "%08x", iParam1);
			snprintf(param2, 11, "%08x", iParam2);
			printf("Navigating\n");
			printf("Finding first occurence of lower bound %s\n", param1);
			result = StringIndex_public_StringIndex_navigateTo(cases, param1);
			printf("Finding first occurence of upper bound %s\n", param2);
			resultend = StringIndex_public_StringIndex_navigateTo(cases, param2);
			
			printf("Results between bounds\n");
			printHead();
			StringIndex_public_iterate_range(cases, result, resultend, NULL, printResult);
			break;
		case 'e':
			printf("Min deaths max deaths?\n");
			scanf("%d %d", &iParam1, &iParam2);
			snprintf(param1, 11, "%04x", iParam1);
			snprintf(param2, 11, "%04x", iParam2);
			printf("Navigating\n");
			printf("Finding first occurence of lower bound\n");
			result = StringIndex_public_StringIndex_navigateTo(deaths, param1);
			printf("Finding first occurence of upper bound\n");
			resultend = StringIndex_public_StringIndex_navigateTo(deaths, param2);
			
			printf("Results between bounds\n");
			printHead();
			StringIndex_public_iterate_range(deaths, result, resultend, NULL, printResult);
			break;
		case 'd':
			printf("Startdate Enddate?\n");
			scanf("%s %s", param1, param2);
			printf("Navigating.\n");
			printf("Finding first occurence of start date\n");
			result = StringIndex_public_StringIndex_navigateTo(dates, param1);
			printf("Finding first occurence of end date\n");
			resultend = StringIndex_public_StringIndex_navigateTo(dates, param2);
			printf("Results between dates\n");
			StringIndex_public_iterate_range(dates, result, resultend, NULL, printResult);
			break;
		case 'c':
			printf("Query?\n");
			scanf("%s", param1);
			printf("Navigating.\n");
			result = StringIndex_public_StringIndex_navigateTo(cities, param1);
			printHead();
			StringIndex_public_iterate(result, NULL, printResult);
			break;
		case 's':
			printf("Query?\n");
			scanf("%s", param1);
			printf("Navigating.\n");
			result = StringIndex_public_StringIndex_navigateTo(states, param1);
			StringIndex_public_iterate(result, NULL, printResult);
			printHead();
			printf("Amount of results: %d\n", result->referencesCount);
			break;
		default:
			break;
		}	
	}

	return 0;
}