#pragma once
#include "arena.h"

/* Codigos das instrucoes */
typedef enum {
    PUSH,       //Empilha argumento na pilha de dados
    POP,        //Desempilha elemento da pilha de dados
    DUP,        //Duplica o topo da pilha de dados
    ADD,        //Soma os dois elementos no topo da pilha
    SUB,        //Subtrai os dois elementos no topo da pilha
    MUL,        //Multiplica os dois elementos no topo da pilha
    DIV,        //Divide os dois elementos no topo da pilha
    JMP,        //Pulo incondicional para argumento
    JIT,        //Pulo se verdadeiro
    JIF,        //Pulo se falso
    CALL,       //Chama a funcao no endereço argumento
    RET,        //Retorna  
    EQ,         //Compara os dois elementos do topo da pilha de dados (=)
    GT,         //Compara os dois elementos do topo da pilha de dados (>)
    GE,         //Compara os dois elementos do topo da pilha de dados (>=)
    LT,         //Compara os dois elementos do topo da pilha de dados (<)
    LE,         //Compara os dois elementos do topo da pilha de dados (<=)
    DIFF,       //Compara os dois elementos do topo da pilha de dados (!=)
    NEG,        //Troca o sinal do topo da pilha
    STO,        //Desempilha elemento da pilha de dados e armazena em mem[arg]
    RCL,        //Empilha elemento de mem[arg] na pilha de dados
    ENTRY,      //Inicia um novo frame
    LEAVE,      //Deleta um frame
    END,        //Finaliza a execução do programa
    PRN,        //Desempilha e imprime o elemento da pilha de dados
    SYS,        //Chamada para o sistema (arena)
    ATR,        //Empilha um atributo de um tile especifico na memoria
    RCM,        //Empilha elemento de mem[topo_da_pilha] na pilha de dados
} OpCode;

/* Tipos dos operandos */
typedef enum {
    NUM,
    ACAO,
    VAR,    // No momento usado para direcoes
    TILE,
    TERRENO,
} Tipo;

typedef enum{
    MOV,  // Mover robo
    REC,  // Recolher cristal
    DEP,  // Depositar cristal
    VER,  // Checar as vizinhanças
    ATQ,  // Atacar as vizinhanças
    JGC,  // Jogar cristal
    KMK,  // Kamikaze
} High_instr;

typedef enum{
    N,  // Norte
    NE, // Nordeste
    SE, // Sudeste
    S,  // Sul
    SW, // Sudoeste
    NW, // Noroeste
    CN, // Centro (posicao propria do robo)
} Dir;

/* Operando */
typedef struct { 
    Tipo t; 
    union { 
        int n; 
        High_instr ac; 
        Dir v; 
        Tile tile;
        Terreno terr;
    } valor; 
} OPERANDO; 

/* Instrucao */
typedef struct {
    OpCode instr;
    OPERANDO op;
} INSTR;

