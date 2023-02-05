#include <stdio.h>
#include <stdlib.h>

// 8-bit signed byte of karakter
char waarde1 = 'a';
char waarde2 = 30;

// Tekenreeks
char *woorden = "Hallo";

// 16-bit signed word
short waarde3 = 4999;

// 32-bit signed dword
int waarde4 = 353524;

// 64-bit signed qword
long waarde5 = 35253325;

// "Objecten" definieren (tevens een voorbeeld van een linked list)
typedef struct datastructuur *StructuurReferentie;
struct datastructuur
{
	int getal;
	char *tekens;
	StructuurReferentie volgende;
};

// Een voorbeeld van een linkedlist
typedef struct listnode *LinkedList;
struct listnode
{
	void *inhoud;			
	// Weet je niet waar je naar gaat verwijzen? 
	// Gebruik void * 
	LinkedList volgende;
	LinkedList vorige;
};

typedef int (*FunctieVerwijzing)(int a, int b);

int voorbeeldFunctie(int a, int b)
{
	return a + b;
}

int voorbeeldFunctie2(int a, int b)
{
	return a - b;
}

int main(int argc, char const *argv[])
{
	// Letterlijk arrays;
	int eenExplicieteArray[10];
	eenExplicieteArray[3] = 65;

	printf("%d\n", eenExplicieteArray[3]);
	printf("%d\n", eenExplicieteArray[11]);
	// Let op: Bovenstaande resulteert in een array met onzin er in; 
	// Dat het geheugen van jou is, wil niet zeggen dat het is opgeruimd.
	// Verder behoed C je er niet van buiten je arraygrenzen te gaan neuzen.
	// Let dus goed op want daar kun je problemen mee krijgen.

	// Geheugen reserveren voor arrays;
	int *eenArrayVanNaderTeBepalenOmvang = calloc(10, sizeof(int));
	eenArrayVanNaderTeBepalenOmvang[4] = 33;

	// Geheugen reserveren voor arrays van structuren
	StructuurReferentie *eenArrayVanObjectAchtigen = calloc(10, sizeof(StructuurReferentie));

	// Geheugen reserveren voor een structuur;
	eenArrayVanObjectAchtigen[3] = malloc(sizeof(struct datastructuur));

	// Waardes ophengelen uit structuren;
	eenArrayVanObjectAchtigen[3]->tekens = calloc(10, sizeof(char));

	printf("De gebruiker iets vertellen...\n");

	// Teksten uit terminal lezen;
	scanf("%10s", eenArrayVanObjectAchtigen[3]->tekens);

	printf("De gebruiker nog iets vertellen...\n");

	// Teksten naar terminal schrijven;
	printf("%10s\n", eenArrayVanObjectAchtigen[3]->tekens);

	// While lussen zoals bekend:
	int aftellen = 10;
	while (aftellen-- > 0)
		printf("%d\n", aftellen);

	// Forlussen evenzo
	for (int i = 0; i < 10; ++i)
	{
		printf("%d\n", i);
	}

	// Met functie-verwijzingen werken (function pointers)
	FunctieVerwijzing onbekendeFunctie = voorbeeldFunctie;
	printf("4+3=%d\n", onbekendeFunctie(4, 3));
	onbekendeFunctie = voorbeeldFunctie2;
	printf("4-3=%d\n", onbekendeFunctie(4, 3));

	// Een array van functie verwijzingen.... wanneer zou dat ooit nuttig zijn?
	FunctieVerwijzing meerdereFuncties[] = {
		voorbeeldFunctie,
		voorbeeldFunctie2
	};

	// Simpel...ongein!
	int a = 4;
	int b = 7;
	meerdereFuncties[a > b](a, b);

	// voidpointers en casten
	void *ikWeetNietWatHetWord;
	ikWeetNietWatHetWord = eenArrayVanObjectAchtigen[3];
	printf("%s\n", ((StructuurReferentie)ikWeetNietWatHetWord)->tekens);

	// Bij elke alloc, hoort een free; 
	free(eenArrayVanNaderTeBepalenOmvang);
	// ook bij elk lid van een array van pointers en diens onderliggende arrays, strings, et cetera.
	free(eenArrayVanObjectAchtigen[3]->tekens);
	// ook bij elk lid van een array van pointers
	free(eenArrayVanObjectAchtigen[3]);
	// dus maak eerst de inhoud leeg voordat het array zelf terziele gaat;
	free(eenArrayVanObjectAchtigen);

	return 0;
}