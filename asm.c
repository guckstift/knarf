#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union Ram {
	void *v;
	char *c;
	int *i;
} Ram;

char *operations[] = {
	"halt", "push", "load", "store", "add", "sub", "mul", "div", "print", "prints", "jz", "jmp",
};

int sourceLen = 0;
char *source = 0;
int numOperations = sizeof(operations) / sizeof(char*);
char *cur = 0;
char *start = 0;
Ram binary = {.v = 0};
Ram curBin = {.v = 0};
int found = 0;
int num = 0;
int tokLen = 0;
int opLen = 0;
FILE *fs = 0;

struct Reloc {
	struct Reloc *next;
	char *name;
	int length;
	int position;
} *relocs = 0, *lastReloc = 0;

struct Label {
	struct Label *next;
	char *name;
	int length;
	int position;
} *labels = 0, *lastLabel = 0;

void addReloc(char *name, int length, int position)
{
	struct Reloc *new = malloc(sizeof(struct Reloc));
	
	new->next = 0;
	new->name = name;
	new->length = length;
	new->position = position;
	
	if(!relocs) {
		relocs = new;
		lastReloc = new;
	}
	else {
		lastReloc->next = new;
		lastReloc = new;
	}
}

void addLabel(char *name, int length, int position)
{
	struct Label *new = malloc(sizeof(struct Label));
	
	new->next = 0;
	new->name = name;
	new->length = length;
	new->position = position;
	
	if(!labels) {
		labels = new;
		lastLabel = new;
	}
	else {
		lastLabel->next = new;
		lastLabel = new;
	}
}

void main(int argc, char **argv)
{
	if(argc < 3) {
		printf("usage: asm <output-file> <input-file>\n");
		exit(-1);
	}
	
	fs = fopen(argv[2], "rb");
	
	if(fs == 0) {
		printf("error: input file not found\n");
		exit(-1);
	}
	
	fseek(fs, 0, SEEK_END);
	sourceLen = ftell(fs);
	source = malloc(sourceLen);
	source[sourceLen] = 0;
	fseek(fs, 0, SEEK_SET);
	fread(source, sizeof(char), sourceLen, fs);
	fclose(fs);
	
	binary.v = malloc(sizeof(int) * 1024);
	curBin.v = binary.v;
	
	cur = source;
	start = source;
	
	while(*cur) {
		if(*cur >= 'a' && *cur <= 'z') {
			start = cur;
			
			while(*cur >= 'a' && *cur <= 'z') cur++;
			
			found = 0;
			tokLen = cur - start;
			
			for(int i=0; i<numOperations; i++) {
				opLen = strlen(operations[i]);
				
				if(opLen == tokLen && strncmp(start, operations[i], opLen) == 0) {
					found = 1;
					printf("%li: %s\n", curBin.c - binary.c, operations[i]);
					*curBin.c++ = i;
					break;
				}
			}
			
			if(!found) {
				addReloc(start, tokLen, curBin.c - binary.c);
				printf("%li: reloc %.*s\n", curBin.c - binary.c, tokLen, start);
				*curBin.i++ = 0;
			}
		}
		else if(*cur >= '0' && *cur <= '9') {
			num = 0;
			
			while(*cur >= '0' && *cur <= '9') {
				num = num * 10 + *cur++ - '0';
			}
			
			printf("%li: %i\n", curBin.c - binary.c, num);
			*curBin.i++ = num;
		}
		else if(*cur  == ':') {
			cur++;
			start = cur;
			
			while(*cur >= 'a' && *cur <= 'z') cur++;
			
			tokLen = cur - start;
			addLabel(start, tokLen, curBin.c - binary.c);
			printf("%li: label %.*s\n", curBin.c - binary.c, tokLen, start);
		}
		else if(*cur  == '"') {
			cur++;
			
			printf("%li: string\n", curBin.c - binary.c);
			
			while(*cur != '"' && *cur != 0) {
				if(*cur == '\\') {
					cur++;
				}
				*curBin.c++ = *cur;
				cur++;
			}
			
			*curBin.c++ = 0;
			
			if(*cur == '"') {
				cur++;
			}
		}
		else if(*cur == ' ' || *cur == '\t' || *cur == '\r' || *cur == '\n') {
			cur++;
		}
		else {
			printf("error(%li): unknown character\n", cur - source);
			exit(-1);
		}
	}
	
	struct Reloc *curReloc = relocs;
	
	while(curReloc) {
		struct Label *curLabel = labels;
		found = 0;
		
		while(curLabel) {
			if(
				curLabel->length == curReloc->length &&
				strncmp(curLabel->name, curReloc->name, curLabel->length) == 0
			) {
				*(int*)(binary.c + curReloc->position) = curLabel->position;
				printf("set relocation at %i to %i\n", curReloc->position, curLabel->position);
				found = 1;
				break;
			}
			
			curLabel = curLabel->next;
		}
		
		if(!found) {
			printf("error: label not found\n");
			exit(-1);
		}
		
		curReloc = curReloc->next;
	}
	
	printf("binary has %li words\n", curBin.c - binary.c);
	
	fs = fopen(argv[1], "wb");
	fwrite(binary.v, sizeof(int), 1024, fs);
	fclose(fs);
}
