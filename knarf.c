#include <stdio.h>

enum {HALT, PUSH, LOAD, STORE, ADD, SUB, PRINT};

int ram[1024] = {
	PUSH, 512,
	PUSH, 11,
	PUSH, 13,
	ADD,
	PUSH, 4,
	SUB,
	STORE,
	PUSH, 512,
	LOAD,
	PRINT,
	HALT,
};

int *ip = ram;
int *sp = ram + 1024;
int running = 1;
int op = 0;
int op1 = 0;
int op2 = 0;

void main()
{
	printf("knarf\n");
	
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
		}
	}
}
