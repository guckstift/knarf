#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *operations[] = {
	"halt", "push", "load", "store", "add", "sub", "print",
};

int sourceLen = 0;
char *source = 0;
int numOperations = sizeof(operations) / sizeof(char*);
char *cur = 0;
char *start = 0;
int binary[1024];
int *curBin = binary;
int found = 0;
int num = 0;
FILE *fs = 0;

void main(int argc, char **argv)
{
	if(argc < 2) {
		printf("error: missing input file\n");
		exit(-1);
	}
	
	fs = fopen(argv[1], "rb");
	
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
	
	cur = source;
	start = source;
	
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
	
	fs = fopen("prog.bin", "wb");
	fwrite(binary, sizeof(int), 1024, fs);
	fclose(fs);
}
