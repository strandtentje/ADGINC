#include "stringindex.h"

typedef struct location* Location;
typedef struct datetime* DateTime;
typedef struct statistic* Statistic;
typedef struct registration* Registration;
typedef struct registrationlist* RegistrationList;

struct datetime {
	char day;		
	char month;
	short year;
};

struct statistic {
	int cases;
	int deaths;
};

struct registration {
	int locationcode;
	StringIndex state;
	StringIndex city;
	DateTime timestamp;
	Statistic statistic;
};

struct registrationlist {
	Registration *array;
	int count;
	int size;
};

RegistrationList rl_create();
void rl_add(RegistrationList list, Registration item);