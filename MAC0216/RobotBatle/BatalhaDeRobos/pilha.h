#pragma once
#include "instr.h"

#define PILMAX 99

/*
 * topo: indica o topo da pilha
 * val: vetor de OPERANDO de tamanho PILMAX
 */
typedef struct {
    int topo;
    OPERANDO val[PILMAX+1];
} Pilha;

/*
 * Retorna uma instancia de uma pilha
 */
Pilha cria_pilha();

/*
 * Coloca um operando no topo da pilha
 *
 * Caso a pilha esteja cheia, imprime erro,
 *     mas nao interrompe a execucao
 */
void empilha(Pilha *p, OPERANDO op);

/*
 * Retorna o operando que estava no topo da
 * pilha e o remove dela (= mudar o topo em
 * uma posicao)
 */
OPERANDO desempilha(Pilha *p);

/*
 * De cima para baixo, imprime todos os
 * elementos da pilha ate um numero
 * arbitrario inteiro
 *
 * Imprime tambem caracteres para auxiliar na leitura
 */
void imprime(Pilha *p, int n);
