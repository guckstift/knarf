#include <stdio.h>
#include <stdlib.h>

enum {HALT, PUSH, LOAD, STORE, ADD, SUB, MUL, DIV, PRINT, JZ, JMP};

typedef union Ram {
	void *v;
	char *c;
	int *i;
} Ram;

FILE *fs = 0;

Ram ram;
Ram ip = {.v = 0};
Ram sp = {.v = 0};
int running = 1;
char op = 0;
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
	
	ram.v = malloc(sizeof(int) * 1024);
	ip.v = ram.v;
	sp.i = ram.i + 1024;
	
	fread(ram.v, sizeof(int), 1024, fs);
	
	while(running) {
		op = *ip.c++;
		
		switch(op) {
			case HALT:
				running = 0;
				break;
			case PUSH:
				op1 = *ip.i++;
				*--sp.i = op1;
				break;
			case LOAD:
				op1 = *sp.i++;
				*--sp.i = *(int*)(ram.c + op1);
				break;
			case STORE:
				op2 = *sp.i++;
				op1 = *sp.i++;
				*(int*)(ram.c + op1) = op2;
				break;
			case ADD:
				op2 = *sp.i++;
				op1 = *sp.i++;
				*--sp.i = op1 + op2;
				break;
			case SUB:
				op2 = *sp.i++;
				op1 = *sp.i++;
				*--sp.i = op1 - op2;
				break;
			case MUL:
				op2 = *sp.i++;
				op1 = *sp.i++;
				*--sp.i = op1 * op2;
				break;
			case DIV:
				op2 = *sp.i++;
				op1 = *sp.i++;
				*--sp.i = op1 / op2;
				break;
			case PRINT:
				op1 = *sp.i++;
				printf("%i\n", op1);
				break;
			case JZ:
				op2 = *sp.i++;
				op1 = *sp.i++;
				if(!op1) ip.c = ram.c + op2;
				break;
			case JMP:
				op1 = *sp.i++;
				ip.c = ram.c + op1;
				break;
			default:
				printf("error: unknown opcode %i\n", op);
				exit(-1);
		}
	}
}
