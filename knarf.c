#include <stdio.h>
#include <stdlib.h>

enum {HALT, PUSH, LOAD, STORE, ADD, SUB, PRINT};

FILE *fs = 0;
int ram[1024];
int *ip = ram;
int *sp = ram + 1024;
int running = 1;
int op = 0;
int op1 = 0;
int op2 = 0;

void main(int argc, char **argv)
{
	printf("knarf\n");
	
	if(argc < 2) {
		printf("error: missing input file\n");
		exit(-1);
	}
	
	fs = fopen(argv[1], "rb");
	
	if(fs == 0) {
		printf("error: input file not found\n");
		exit(-1);
	}
	
	fread(ram, sizeof(int), 1024, fs);
	
	while(running) {
		op = *ip++;
		
		switch(op) {
			case HALT:
				running = 0;
				break;
			case PUSH:
				op1 = *ip++;
				*--sp = op1;
				break;
			case LOAD:
				op1 = *sp++;
				*--sp = ram[op1];
				break;
			case STORE:
				op2 = *sp++;
				op1 = *sp++;
				ram[op1] = op2;
				break;
			case ADD:
				op2 = *sp++;
				op1 = *sp++;
				*--sp = op1 + op2;
				break;
			case SUB:
				op2 = *sp++;
				op1 = *sp++;
				*--sp = op1 - op2;
				break;
			case PRINT:
				op1 = *sp++;
				printf("%i\n", op1);
				break;
			default:
				printf("error: unknown opcode %i\n", op);
				exit(-1);
		}
	}
}
