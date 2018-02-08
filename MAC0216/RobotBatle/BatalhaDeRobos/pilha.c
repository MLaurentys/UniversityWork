#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"
#include "maq.h"

/*
 * Retorna uma instancia de uma pilha
 */
Pilha cria_pilha() {
    Pilha p;
    p.topo = 0;
    return p;
}

/*
 * Coloca um operando no topo da pilha
 *
 * Caso a pilha esteja cheia, imprime erro,
 *     mas nao interrompe a execucao
 */
void empilha(Pilha *p, OPERANDO op) {
    if (p->topo < PILMAX)
        p->val[p->topo++] = op;
    else Erro("Pilha cheia");
}


/*
 * Retorna o operando que estava no topo da
 * pilha e o remove dela (= mudar o topo em
 * uma posicao)
 */
OPERANDO desempilha(Pilha *p) {
    if (p->topo > 0)
        return p->val[--p->topo];
    else {
        Erro("Pilha vazia");
        return cria_operando(NUM, -1);
    }
}


/*
 * De cima para baixo, imprime todos os
 * elementos da pilha ate um numero
 * arbitrario inteiro
 *
 * Imprime tambem caracteres para auxiliar na leitura
 */
void imprime(Pilha *p, int n) {
    int t = p->topo-1;
    int i;
    printf("[");
    for (i = t; i >= 0 && i > p->topo-n; i--){ 
    printf(" ");
    imprime_op(p->val[i]);
    printf(", ");
    }
    printf("]");
    return;
}
