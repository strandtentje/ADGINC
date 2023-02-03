#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringindex.h"

char StringIndex_private_static_char_sanitize(char cc) { // Given Character
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

StringIndex StringIndex_public_static_StringIndex_constructor()
{
	StringIndex empty = malloc(sizeof(struct stringindex));			
	memset(empty, 0, sizeof(struct stringindex));

	empty->references = calloc(1, sizeof(int));
	empty->referencesSize = 1;
	empty->referencesCount = 0;
	return empty;
}

StringIndex StringIndex_private_static_StringIndex_insert(StringIndex head, StringIndex tail, char cc)
{
	StringIndex inserted = StringIndex_public_static_StringIndex_constructor();
	if (head != NULL)
		head->next = inserted;
	if (tail != NULL)
		tail->previous = inserted;
	inserted->previous = head;
	inserted->next = tail;	
	inserted->letter = cc;	
	return inserted;
}

StringIndex StringIndex_private_StringIndex_getDown(StringIndex this)
{	
	if (this->down != NULL) return this->down;
	this->down = StringIndex_public_static_StringIndex_constructor();
	return this->down;
}

StringIndex StringIndex_private_StringIndex_findNode(
	StringIndex this, 
	char *string, 
	int position)
{	
	char sanitized = StringIndex_private_static_char_sanitize(string[position]);
	if (sanitized == '\0') return this;	
	
	if (sanitized > this->letter) {		
		if (this->next == NULL)
			this = StringIndex_private_static_StringIndex_insert(this, this->next, sanitized);		
		else 
			this = this->next;
	} else if (sanitized < this->letter) {
		this = StringIndex_private_static_StringIndex_insert(this->previous, this, sanitized);	
	} else {		
		position++;
		this = StringIndex_private_StringIndex_getDown(this);
	}	
	return StringIndex_private_StringIndex_findNode(this, string, position);
}

void StringIndex_private_StringIndex_resize(StringIndex this) {	
	this->referencesSize = this->referencesSize * 2 + 1;	
	this->references = realloc(this->references, this->referencesSize * sizeof(int));	
}

char *StringIndex_public_string_setText(StringIndex this, char *string) {	
	if (this->text == NULL)		
		this->text = string;
	
	return this->text;
}

void StringIndex_public_addReference(StringIndex this, int reference) {
	if (this->referencesCount >= this->referencesSize) 
		StringIndex_private_StringIndex_resize(this);
	
	this->references[this->referencesCount++] = reference;	
}

StringIndex StringIndex_public_StringIndex_navigateTo(StringIndex this, char *string) {
	return StringIndex_private_StringIndex_findNode(this, string, 0);
}


StringIndex StringIndex_public_StringIndex_wildcardNavigate(StringIndex this, char *query) {
	for (char *i = query; *i != '\0'; i++)
		if (*i == '*') *i = '\0';
	return StringIndex_public_StringIndex_navigateTo(this, query);
}

void StringIndex_public_iterate(
	StringIndex this, 
	void* param, 
	void (*iterator)(StringIndex item, void* param)) {
	iterator(this, param);
	if (this->down != NULL)
		StringIndex_public_iterate(this->down, param, iterator);
	if (this->next != NULL)
		StringIndex_public_iterate(this->next, param, iterator);
}