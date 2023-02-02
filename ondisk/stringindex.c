#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringindex.h"

char csi_sanitize(char cc) { // Given Character
	if (cc == '\0')		 // When it is a null terminator
		return '\0';     // Preserve it or we have a string that will never end
	if (cc > 'Z') 		 // When above capitals range
		cc -= 32; 		 // Then Attempt capitalization	
	if (cc < 'A')		 // When now below capitals range
		return '_';		 // Then Default	
	if (cc > 'Z')		 // When still above capitals range
		return '|';		 // Then Default	
	return cc;		     // When Capital, return capital.
}

StringIndex osi_createEmpty()
{
	StringIndex empty = malloc(sizeof(struct stringindex));			
	memset(empty, 0, sizeof(struct stringindex));

	empty->references = calloc(1, sizeof(int));
	empty->referencesSize = 1;
	empty->referencesCount = 0;
	return empty;
}

StringIndex csi_insert(StringIndex head, StringIndex tail, char cc)
{
	StringIndex inserted = osi_createEmpty();
	if (head != NULL)
		head->next = inserted;
	if (tail != NULL)
		tail->previous = inserted;
	inserted->previous = head;
	inserted->next = tail;	
	inserted->letter = cc;	
	return inserted;
}

StringIndex csi_insertBelow(StringIndex node)
{	
	if (node->down != NULL) return node->down;
	node->down = osi_createEmpty();
	return node->down;
}

StringIndex csi_findNode(
	StringIndex index, 
	char *string, 
	int position)
{	
	char sanitized = csi_sanitize(string[position]);
	if (sanitized == '\0') return index;	
	
	if (sanitized > index->letter) {		
		if (index->next == NULL)
			index = csi_insert(index, index->next, sanitized);		
		else 
			index = index->next;
	} else if (sanitized < index->letter) {
		index = csi_insert(index->previous, index, sanitized);	
	} else {		
		position++;
		index = csi_insertBelow(index);
	}	
	return csi_findNode(index, string, position);
}

void csi_resize(StringIndex index) {	
	index->referencesSize = index->referencesSize * 2 + 1;	
	index->references = realloc(index->references, index->referencesSize * sizeof(int));	
}

char *osi_setText(StringIndex index, char *string) {	
	if (index->text == NULL)		
		index->text = string;
	
	return index->text;
}

void osi_addReference(StringIndex index, int reference) {
	if (index->referencesCount >= index->referencesSize) 
		csi_resize(index);
	
	index->references[index->referencesCount++] = reference;	
}

StringIndex osi_navigateTo(StringIndex index, char *string) {
	return csi_findNode(index, string, 0);
}
