#include <stdlib.h>
#include <stdio.h>

#ifndef STRINGINDEX_
#define STRINGINDEX_
typedef struct stringindex* StringIndex;

struct stringindex {
	char letter;
	StringIndex previous;
	StringIndex next;
	StringIndex down;

	char *text;
	int *references;
	int referencesSize;
	int referencesCount;
};

char 		StringIndex_private_static_char_sanitize(char cc);
StringIndex StringIndex_private_static_StringIndex_insert(StringIndex head, StringIndex tail, char cc);
StringIndex StringIndex_private_StringIndex_getDown(StringIndex this);
StringIndex StringIndex_private_StringIndex_findNode(StringIndex this, char *string, int position);
void	 	StringIndex_private_StringIndex_resize(StringIndex this);

StringIndex StringIndex_public_StringIndex_navigateTo(StringIndex this, char *string);
StringIndex StringIndex_public_static_StringIndex_constructor();
void 		StringIndex_public_addReference(StringIndex this, int reference);
char 	   *StringIndex_public_string_setText(StringIndex this, char *string);
StringIndex StringIndex_public_StringIndex_wildcardNavigate(StringIndex this, char *query);
void 		StringIndex_public_iterate(StringIndex this, void* param, void (*iterator)(StringIndex item, void* param));
#endif