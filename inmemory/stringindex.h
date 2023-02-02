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

char csi_sanitize(char cc);
StringIndex csi_insert(StringIndex head, StringIndex tail, char cc);
StringIndex csi_insertBelow(StringIndex node);
StringIndex csi_findNode(StringIndex index, char *string, int position);
void csi_resize(StringIndex index);

StringIndex osi_navigateTo(StringIndex index, char *string);
StringIndex osi_createEmpty();
void osi_addReference(StringIndex index, int reference);
char *osi_setText(StringIndex index, char *string);
#endif