#include "stringindex.h"
#ifndef COVID_
#define COVID_
typedef struct location* Location;
typedef struct statistic* Statistic;
typedef struct registration* Registration;
typedef struct temporaryRegistration* TemporaryRegistration;
typedef struct registrationlist* RegistrationList;

struct temporaryRegistration {
	char *city;
	char *state;
	char *year;
	char *month;
	char *day;
	char *isoDate;
	char *hexCases;
	char *hexDeaths;
	int cases;
	int deaths;
};

struct registration {
	int primaryKey;
	int locationcode;
	StringIndex state;
	StringIndex city;
	StringIndex date;
	StringIndex cases;
	StringIndex deaths;
};

struct registrationlist {
	Registration *array;
	int count;
	int size;
};

TemporaryRegistration tmp_create();
char *tmp_deriveIsoDate(TemporaryRegistration this);
char *tmp_indexibleCases(TemporaryRegistration this);
char *tmp_indexibleDeaths(TemporaryRegistration this);
char *tmp_getPermanentCity(void *uncast);
char *tmp_getPermanentState(void *uncast);
char *tmp_getPermanentDate(void *uncast);
char *tmp_getLegibleCases(void *uncast);
char *tmp_getLegibleDeaths(void *uncast);

Registration r_create();
RegistrationList rl_create();
int rl_add(RegistrationList list, Registration item);
#endif