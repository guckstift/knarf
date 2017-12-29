#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char source[] =
	"push 512 "
	"push 11 push 13 add "
	"push 4 sub "
	"store "
	"push 512 load "
	"print "
	"halt "
;

char *operations[] = {
	"halt", "push", "load", "store", "add", "sub", "print",
};

int numOperations = sizeof(operations) / sizeof(char*);
char *cur = source;
char *start = source;
int binary[1024];
int *curBin = binary;
int found = 0;
int num = 0;
FILE *fs;

void main()
{
	while(*cur) {
		if(*cur >= 'a' && *cur <= 'z') {
			start = cur;
			
			while(*cur >= 'a' && *cur <= 'z') cur++;
			
			found = 0;
			
			for(int i=0; i<numOperations; i++) {
				int opLen = strlen(operations[i]);
				int tokLen = cur - start;
				
				if(opLen == tokLen && strncmp(start, operations[i], opLen) == 0) {
					found = 1;
					*curBin++ = i;
					printf("%s\n", operations[i]);
					break;
				}
			}
			
			if(!found) {
				printf("error(%li): unknown operation\n", cur - source);
				exit(-1);
			}
		}
		else if(*cur >= '0' && *cur <= '9') {
			num = 0;
			
			while(*cur >= '0' && *cur <= '9') {
				num = num * 10 + *cur++ - '0';
			}
			
			*curBin++ = num;
			printf("%i\n", num);
		}
		else if(*cur == ' ' || *cur == '\t' || *cur == '\r' || *cur == '\n') {
			cur++;
		}
		else {
			printf("error(%li): unknown character\n", cur - source);
			exit(-1);
		}
	}
	
	printf("binary has %li words\n", curBin - binary);
	
	fs = fopen("prog.bin", "w");
	fwrite(binary, sizeof(int), 1024, fs);
	fclose(fs);
}
