#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stringindex.h"
#include "testing.h"

void test_sanitize()
{
	introduce("test_sanitize");
	char_equals("Given Lowercase A, Then Makes Uppercase A", csi_sanitize('a'), 'A');
	char_equals("Given Lowercase Z, Then Makes Uppercase Z", csi_sanitize('z'), 'Z');
	char_equals("Given Uppercase A, Then Makes Uppercase A", csi_sanitize('A'), 'A');
	char_equals("Given Uppercase Z, Then Makes Uppercase A", csi_sanitize('Z'), 'Z');
	char_equals("Given Number, Then Defaults              ", csi_sanitize('4'), '_');
	char_equals("Given Special Char, Then Defaults        ", csi_sanitize('#'), '_');
	char_equals("Given Linebreak, Then Defaults           ", csi_sanitize('\n'), '_');
	char_equals("Given Default, Then Preserves            ", csi_sanitize('_'), '_');
	char_equals("Given 0 terminator, Then Preserves       ", csi_sanitize('\0'), '\0');
}

void test_initialize()
{
	introduce("test_initialize");
	StringIndex empty = osi_createEmpty();
	int_equals("letter is 0 terminator    ", empty->letter == '\0', 1);
	int_equals("previous is null reference", empty->previous == NULL, 1);
	int_equals("next is null reference    ", empty->next == NULL, 1);
	int_equals("down is null reference    ", empty->down == NULL, 1);
	int_equals("text is null reference    ", empty->text == NULL, 1);
	int_equals("reference size is 1       ", empty->referencesSize, 1);
	int_equals("reference count is 0      ", empty->referencesCount, 0);
	empty->references[0] = 22;
	int_equals("references value is kept  ", empty->references[0], 22);
}

void test_insert_tail()
{
	introduce("test_insert_tail");
	StringIndex head = osi_createEmpty();
	head->letter = 'A';
	StringIndex tail = csi_insert(head, NULL, 'B');	
	char_equals("Given Letter is kept         ", tail->letter, 'B');
	char_equals(" Previous Letter is accessible", tail->previous->letter, 'A');
	char_equals(" Previous Next letter is own  ", tail->previous->next->letter, 'B');
	int_equals (" Next on head is inserted tail", head->next == tail, 1);
	int_equals (" Prev on inserted tail is head", tail->previous == head, 1);
	int_equals (" Next on tail is NULL         ", tail->next == NULL, 1);
}

void test_insert_head()
{
	introduce("test_insert_head");
	StringIndex tail = osi_createEmpty();
	tail->letter = 'A';
	StringIndex head = csi_insert(NULL, tail, 'B');	
	char_equals("Given Letter is kept             ", head->letter, 'B');
	char_equals("Next Letter is accessible        ", head->next->letter, 'A');
	char_equals("Next Previous Letter is own      ", head->next->previous->letter, 'B');
	int_equals ("Previous on tail is inserted head", tail->previous == head, 1);
	int_equals ("Next on inserted head is tail    ", head->next == tail, 1);
	int_equals ("Previous on inserted head is NULL", head->previous == NULL, 1);
}

void test_insert_between()
{
	introduce("test_insert_between");
	StringIndex head = osi_createEmpty();
	StringIndex tail = osi_createEmpty();
	head->letter = 'A';
	tail->letter = 'C';
	StringIndex betw = csi_insert(head, tail, 'B');
	char_equals("Head letter is kept               ", head->letter, 'A');
	char_equals("Next on head is inserted letter   ", head->next->letter, 'B');
	char_equals("Next next on head is tail letter  ", head->next->next->letter, 'C');
	char_equals("Tail letter is kept               ", tail->letter, 'C');
	char_equals("Prev on tail is inserted letter   ", tail->previous->letter, 'B');
	char_equals("Prev prev on tail is head letter  ", tail->previous->previous->letter, 'A');
	char_equals("Inserted letter is kept           ", betw->letter, 'B');
	char_equals("Prev letter on inserted is head   ", betw->previous->letter, 'A');
	char_equals("Next letter on inserted is tail   ", betw->next->letter, 'C');

	int_equals ("Head has no previous     ", head->previous == NULL, 1);
	int_equals ("Tail has no next         ", tail->next == NULL, 1);
	int_equals ("Head has inserted as next", head->next == betw, 1);
	int_equals ("Tail has inserted as prev", tail->previous == betw, 1);
	int_equals ("Next next head is tail   ", head->next->next == tail, 1);
	int_equals ("Prev prev tail is head   ", tail->previous->previous == head, 1);
	int_equals ("Prev on inserted is head ", betw->previous == head, 1);
	int_equals ("Next on inserted is tail ", betw->next == tail, 1);
}

void test_down_node_insertion()
{
	introduce("test_down_node_insertion");
	StringIndex up = osi_createEmpty();
	StringIndex down1 = csi_insertBelow(up);
	int_equals("Down is assgined correctly           ", up->down == down1, 1);
	StringIndex down2 = csi_insertBelow(up);
	int_equals("Existing down is returned on existing", down1 == down2, 1);
	int_equals("Down is not overwritten on existing  ", up->down == down1, 1);
}

void test_find()
{
	introduce("test_find");
	StringIndex indices = osi_createEmpty();

	StringIndex helloIndex = csi_findNode(indices, "Hello", 0);
	StringIndex hellishIndex = csi_findNode(indices, "Hellish", 0);
	StringIndex worldIndex = csi_findNode(indices, "World", 0);	

	StringIndex searchHelloIndex = csi_findNode(indices, "Hello", 0);
	StringIndex searchHellishIndex = csi_findNode(indices, "Hellish", 0);
	StringIndex searchWorldIndex = csi_findNode(indices, "World", 0);
	StringIndex searchHellIndex = csi_findNode(indices, "Hell", 0);

	int_equals	("initial index result matches search result index on split ", helloIndex == searchHelloIndex, 1);
	int_equals	("initial index result matches search result index on split ", hellishIndex == searchHellishIndex, 1);
	int_equals	("initial index result matches search result index on orphan", worldIndex == searchWorldIndex, 1);
	char_equals	("letters on split indexed correctly                        ", searchHellIndex->next->letter, 'I');
	char_equals	("letters on split indexed ordinally                        ", searchHellIndex->next->next->letter, 'O');
	char_equals	("letter on end of word node is null terminator             ", searchHellIndex->next->next->down->letter, '\0');
	int_equals	("manual navigation validation                              ", searchHellIndex->next->next->down == searchHelloIndex, 1);
}

void test_set_text()
{
	introduce("test_set_text");
	StringIndex indices = osi_createEmpty();

	char *helloString = "hello";
	StringIndex helloIndex = csi_findNode(indices, helloString, 0);
	char *indexedString = osi_setText(helloIndex, helloString);

	int_equals(
		"initial set text used existing string allocation    ", 
		helloString == indexedString, 1);

	char *newHelloString = "HELLO";
	StringIndex foundIndex = csi_findNode(indices, newHelloString, 0);
	char *foundString = osi_setText(foundIndex, newHelloString);

	int_equals(
		"attempted overwrite doesnt use new string allocation", 
		newHelloString != foundString, 1);
	int_equals(
		"attempted overwrite uses existing string allocation ",
		helloString == foundString, 1);
}

void test_add_reference() 
{
	introduce("test_add_reference");
	StringIndex indices = osi_createEmpty();

	StringIndex helloIndex = csi_findNode(indices, "Hello", 0);

	int_equals("References size is at default of 1 ", helloIndex->referencesSize, 1);
	int_equals("References count is at 0           ", helloIndex->referencesCount, 0);
	osi_addReference(helloIndex, 2);
	int_equals("References size is still at default", helloIndex->referencesSize, 1);
	int_equals("References count is at 1           ", helloIndex->referencesCount, 1);
	osi_addReference(helloIndex, 5);
	int_equals("References size has increased to 3 ", helloIndex->referencesSize, 3);
	int_equals("References count is at 2           ", helloIndex->referencesCount, 2);
	osi_addReference(helloIndex, 23);
	int_equals("References size has remained at 3  ", helloIndex->referencesSize, 3);
	int_equals("References count is at 3           ", helloIndex->referencesCount, 3);
	osi_addReference(helloIndex, 6);
	int_equals("References size has increased to 7", helloIndex->referencesSize, 7);
	int_equals("References count is at 4          ", helloIndex->referencesCount, 4);

	int_equals("First reference  is  2", helloIndex->references[0], 2);
	int_equals("Second reference is  5", helloIndex->references[1], 5);
	int_equals("Third reference  is 23", helloIndex->references[2], 23);
	int_equals("Fourth reference is  4", helloIndex->references[3], 6);
}

int main(int argc, char** argv[])
{
	test_test();
	test_sanitize();
	test_initialize();
	test_insert_tail();
	test_insert_head();
	test_insert_between();
	test_down_node_insertion();
	test_find();
	test_set_text();
	test_add_reference();

	show_results();
}