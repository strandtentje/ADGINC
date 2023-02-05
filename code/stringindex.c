#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringindex.h"
#include <assert.h>

char StringIndex_tokenizeOnlyCapitals(char cc) { // Given Character	
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

char StringIndex_tokenizeOnlyNumbers(char cc) {
	if (cc == '\0')
		return '\0';
	if (cc < '0')
		return '_';
	if (cc > '9')
		return '_';
	return cc;
}

char StringIndex_tokenizeOnlyHex(char cc) {
	if (cc == '\0')
		return '\0';
	if (cc >= 'a' && cc <= 'f')
		return cc - 32;
	if (cc >= 'A' && cc <= 'F')
		return cc;
	if (cc >= '0' && cc <= '9')
		return cc;
	return '_';
}

StringIndex StringIndex_public_static_StringIndex_constructor(TokenizerDelegate tokenize)
{
	StringIndex empty = malloc(sizeof(struct stringindex));			
	memset(empty, 0, sizeof(struct stringindex));

	empty->references = calloc(1, sizeof(int));
	empty->referencesSize = 1;
	empty->referencesCount = 0;
	empty->tokenize = tokenize;

	return empty;
}

StringIndex StringIndex_private_static_StringIndex_insert(StringIndex head, StringIndex tail, TokenizerDelegate tokenize, char cc)
{
	StringIndex inserted = StringIndex_public_static_StringIndex_constructor(tokenize);
	if (head != NULL && tail != NULL) {
		assert(head->up == tail->up);
	}
	if (head != NULL) {
		head->next = inserted;
		inserted->up = head->up;
	}
	if (tail != NULL) {
		tail->previous = inserted;
		inserted->up = tail->up;
	}
	inserted->previous = head;
	inserted->next = tail;	
	inserted->letter = cc;	
	return inserted;
}

StringIndex StringIndex_private_StringIndex_getDown(StringIndex this)
{	
	if (this->down == NULL) {
		this->down = StringIndex_public_static_StringIndex_constructor(this->tokenize);
		this->down->up = this;
	}
	return this->down;
}

StringIndex StringIndex_private_StringIndex_findNode(
	StringIndex this, 
	char *string, 
	int position)
{	
	char sanitized = this->tokenize(string[position]);	
	StringIndex nextNode = this;
	if (sanitized == '\0') return nextNode;	
	
	if (sanitized > nextNode->letter) {		
		if (nextNode->next == NULL)
			nextNode = StringIndex_private_static_StringIndex_insert(nextNode, nextNode->next, this->tokenize, sanitized);		
		else 
			nextNode = nextNode->next;
	} else if (sanitized < nextNode->letter) {
		nextNode = StringIndex_private_static_StringIndex_insert(nextNode->previous, nextNode, this->tokenize, sanitized);	
	} else {		
		position++;
		nextNode = StringIndex_private_StringIndex_getDown(nextNode);
	}	
	return StringIndex_private_StringIndex_findNode(nextNode, string, position);
}


void StringIndex_private_StringIndex_resize(StringIndex this) {	
	this->referencesSize = this->referencesSize * 2 + 1;	
	this->references = realloc(this->references, this->referencesSize * sizeof(int));	
}

char *StringIndex_public_string_sourceText(StringIndex this, SourceTextDelegate textSource, void *param) {	
	if (this->text == NULL)		
		this->text = textSource(param);
	
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
void StringIndex_public_print(
	StringIndex this,
	int depth
) {
	if (this == NULL) return;
	if (depth > 0)
		printf("%*c %c %s \n", depth, ' ', this->letter, this->text);
	else 
		printf("%c %s \n", this->letter, this->text);
	StringIndex_public_print(this->down, depth + 1);
	StringIndex_public_print(this->next, depth);
}

void StringIndex_public_iterate(
	StringIndex this, 
	void* param, 
	void (*iterator)(StringIndex item, void* param)) {
	StringIndex_public_iterate_range(this, this, NULL, param, iterator);
}

void StringIndex_public_iterate_range_imp(
	StringIndex this,
	StringIndex from,
	StringIndex to,
	int *state,
	void *param,
	void (*iterator)(StringIndex item, void* param)) {
	if (this == NULL) return;
	if (this == from) *state = 1;
	if (this == to) *state = 2;
	if (*state == 1) iterator(this, param);
	if (*state == 2) return;
	StringIndex_public_iterate_range_imp(this->down, from, to, state, param, iterator);
	StringIndex_public_iterate_range_imp(this->next, from, to, state, param, iterator);	
}

void StringIndex_public_iterate_range(
	StringIndex this,
	StringIndex from,
	StringIndex to,
	void *param,
	void (*iterator)(StringIndex item, void* param)) {
	int state = 0;
	StringIndex_public_iterate_range_imp(this, from, to, &state, param, iterator);
}