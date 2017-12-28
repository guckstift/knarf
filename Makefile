all: knarf asm

knarf: knarf.c
	gcc -o knarf knarf.c

asm: asm.c
	gcc -o asm asm.c
