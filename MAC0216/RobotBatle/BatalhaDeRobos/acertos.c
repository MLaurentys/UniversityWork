#include "acertos.h"

static int Pilha[20];
static int top = 0;

INSTR Pilha2[30];
static int top2 = 0;

int salva_end(int ip) {
	if (top < 19) {
		Pilha[++top] = ip;
		return 1;
	}
	return 0;
}

int pega_end() {
	if (top >0)
		return Pilha[top--];
	return -1;
}

int pega_atu() {
	return top2;
}

void push(INSTR ins) {
    if (top2 < 30)
    	Pilha2[top2++] = ins;
}

INSTR pop() {
    return Pilha2[--top2];
}