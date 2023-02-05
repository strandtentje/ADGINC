#include <stdlib.h>
#include <stdio.h>
#include "testing.h"

int bad = 0;
int good = 0;

void char_equals(char *gherkin, char actual, char expected)
{
	if (expected != actual) {
		printf("FAIL: expected %c, got %c\n==>", expected, actual);
		bad++;
	} else {
		printf("OK:");
		good++;
	}
	printf(" %s\n", gherkin);
}

void int_equals(char *gherkin, int actual, int expected) 
{
	if (expected != actual) {
		printf("FAIL: expected %d, got %d\n==>", expected, actual);
		bad++;
	} else {
		printf("OK:");
		good++;
	}	
	printf(" %s\n", gherkin);
}

void introduce(char *functionname)
{
	printf("\n\n%s\n", functionname);
	printf("=======================================\n");	
}

void test_test()
{
	introduce("test_test");
	int_equals (" int equality should fail", 1, 0);
	int_equals (" int equality should ok  ", 1, 1);
	char_equals("char equality should fail", 'A', 'B');
	char_equals("char equality should ok  ", 'A', 'A');
}

void show_results()
{	
	printf("Success: %d | Failure : %d\n", good, bad);
}