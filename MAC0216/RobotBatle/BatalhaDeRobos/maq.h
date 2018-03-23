#pragma once
#include "pilha.h"

#define MAXMEM 100
#define MAXFRAMES 30

/*
 * "pil" = pilha de dados
 * "exec" = pila de execucao
 * "id" = nome da maquina
 * "posx" = posicao relativa as 'linhas' do mapa
 * "posy" = posicao relativa as 'colunas' do mapa
 * ip = aponta para o proxima instrucao a ser executada
 * rbp = localizacao do endereco de retorno atual
 * time = no futuro, identificacao do time do robo
 * instrSize = tamanho da pilha de instrucoes
 * prog = pilha de instrucoes
 * cristais = quantos cristais o robo esta carregando
 * vida = quanta vida o robo ainda tem (comeca em 100)
 * cont = contador de ocupacao do robo: determina se ele
 * pode executar seu programa no proximo turno
 */
typedef struct {
    Pilha pil;
    Pilha exec;
    OPERANDO Mem[MAXMEM];
    INSTR *prog;
    int ip;
    int instrSize;
    int rbp;
    int pilret[MAXFRAMES];
    int id;
    int time;
    int cont;
    int vida;
    int position[2];
    int cristais;
} Maquina;


/*
 * Instancia, inicializa e retorna maquina
 */
Maquina cria_maquina(INSTR *p, int iSize, int id, int posx, int posy, int vida, int time);


// Costumava "desalocar" o robo
void destroi_maquina(Maquina m);

int new_frame(Maquina *m, int pos);

int del_frame(Maquina *m);

/*
 * Declarando as variaveis globais que auxiliam
 * na impressao dos dados
 */
extern char *CODES[];
extern char *Chamadas[];
extern char *Direcao[];
extern char *Terrenos[];
extern char *Tipos[];


/*
 * Local onde o robo executa sua rotina
 *
 * Interpreta um instrucao por vez e realiza sua funcao
 * 
 * Caso o IP chegue ao fim do vetor de instrucoes, ele
 * retorna a instrucao 0, independente do que acontecia
 *
 * Tanto instrucoes de alto nivel, como MOV, quanto
 * instrucoes de baixo nivel, como PUSH, sao processadas
 * nessa funcao
 *
 * A funcao executa a interpretacao ate um maximo de n
 * instrucoes. De qualquer forma, um chamada ao sistema
 * interrompe imediatamente o funcionamento do robo.
 * Isso faz com que a arena, apos fornecer o feedback
 * de sua chamada, ceda a vez para outro robo
 */
void exec_maquina(Maquina *m, int n);

/*
 * Retorna um operando tendo como base o tipo e o numero 
 * passados como argumentos
 */
OPERANDO cria_operando(Tipo t, int arg);

/*
 * Retorna uma instrucao tendo como base o 
 * OpCode e Operando passados como argumentos
 */
INSTR cria_instr(OpCode op, OPERANDO arg);

/*
 * Imprime o valor numerico dooperando de uma
 * maneira legivel
 */
void imprime_op(OPERANDO arg);

/*
 * Funcao auxiliar que imprime as primeiras
 * 10 posicoes da memoria do robo
 */
void imprime_mem(Maquina *m);

/*
 * Imprime mensagem na saida de erro
 */
void Erro(char *msg);

/*
 * Imprime mensagem na saida de erro e interrompe
 * o programa, com codigo de saida
 */
void Fatal(char *msg, int cod);