#include <stdlib.h>
#include <stdio.h>

#ifndef STRINGINDEX_
#define STRINGINDEX_

typedef struct stringindex* StringIndex;
typedef char  (*TokenizerDelegate)(char cc);
typedef void  (*IteratorDelegate)(StringIndex item, void* param);
typedef char *(*SourceTextDelegate)(void *param);

struct stringindex {
	char letter;
	StringIndex previous;
	StringIndex next;
	StringIndex up;
	StringIndex down;

	char *text;
	int *references;
	int referencesSize;
	int referencesCount;

	TokenizerDelegate tokenize;
};

char 		StringIndex_private_static_char_sanitize(char cc);
StringIndex StringIndex_private_static_StringIndex_insert(StringIndex head, StringIndex tail, TokenizerDelegate tokenize, char cc);
StringIndex StringIndex_private_StringIndex_getDown(StringIndex this);
StringIndex StringIndex_private_StringIndex_findNode(StringIndex this, char *string, int position);
void	 	StringIndex_private_StringIndex_resize(StringIndex this);

StringIndex StringIndex_public_StringIndex_navigateTo(StringIndex this, char *string);
StringIndex StringIndex_public_static_StringIndex_constructor(TokenizerDelegate tokenize);
void 		StringIndex_public_addReference(StringIndex this, int reference);
char 	   *StringIndex_public_string_sourceText(StringIndex this, SourceTextDelegate textSource, void *param);
StringIndex StringIndex_public_StringIndex_wildcardNavigate(StringIndex this, char *query);
void 		StringIndex_public_iterate(StringIndex this, void* param, IteratorDelegate iterate);
void		StringIndex_public_iterate_range(StringIndex this, StringIndex from, StringIndex to, void* param, IteratorDelegate iterate);

void 		StringIndex_public_print(StringIndex this, int depth);

char 		StringIndex_tokenizeOnlyCapitals(char cc);
char 		StringIndex_tokenizeOnlyNumbers(char cc);
char 		StringIndex_tokenizeOnlyHex(char cc);
#endif