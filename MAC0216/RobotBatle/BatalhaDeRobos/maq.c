#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arena.h"
#include "maq.h"


/*
 * Possibilita executar a maquina em
 * estado de debug.
 *
 * Nesse estado, a maquina realizara
 * todas as operacoes da mesma forma
 * porem imprimira muitas informacoes
 * uteis com o intuito de axiliar no
 * debug do robo. Ela imprime, por
 * exemplo, os estado de ambas as pilhas
 */
#define DEBUG
#ifdef DEBUG
    #define D(X) X
#else
#define D(X)
#endif


/*
 * As seguintes listas tem como unica funcao
 * auxiliar o processo de impressao dos 
 * respectivos termos
 */ 
 
 
/*
 * Enumerador para as instrucoes da
 * maquina
 */
 char *CODES[] = {
    "PUSH",
    "POP",
    "DUP",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "JMP",
    "JIT",
    "JIF",
    "CALL",
    "RET",
    "EQ",
    "GT",
    "GE",
    "LT",
    "LE",
    "DIFF",
    "NEG",
    "STO",
    "RCL",
    "ENTRY",
    "LEAVE",
    "END",
    "PRN",
    "SYS",
    "ATR",
    "RCM",
};


/*
 * Enumerador para as instrucoes que exigem
 * interpretacao da maquina virtual (ou,
 * chamadas de sistema)
 */
char *Chamadas[] = {
    "MOV",
    "REC",
    "DEP",
    "VER",
    "ATQ",
    "JGC",
    "KMK",
};


/*
 * Enumerador para as direcoes relativas ao robo
 * CN é onde o robo se encontra (centro)
 */
char *Direcao[] = {
    "N",
    "NE",
    "SE",
    "S",
    "SW",
    "NW",
    "CN",
};


/*
 * Enumerador para o tipo de terreno que um "Tile"
 * pode apresentar. O tipo do terreno esta 
 * relacionado ao custo de transpo-lo
 */
char *Terrenos[] = {
    "ESTRADA",
    "GRAMA",
    "AREIA",
    "AGUA",
    "MONTANHA",
};

/*
 * Enumerador para os tipos de valores que
 * um operando pode apresentar
 */
char *Tipos[] = {
    "NUM",
    "ACAO",
    "VAR",
    "TILE",
    "TERRENO"
};


/*
 * Imprime mensagem na saida de erro
 */
void Erro(char *msg) {
    fprintf(stderr, "%s\n", msg);
}


/*
 * Imprime mensagem na saida de erro e interrompe
 * o programa, com codigo de saida
 */
void Fatal(char *msg, int cod) {
    Erro(msg);
    exit(cod);
}

/*
 * Instancia, inicializa e retorna maquina
 *
 * "pil" = pilha de dados
 * "exec" = pila de execucao
 * "id" = nome da maquina
 * "posx" = posicao relativa as 'linhas' do mapa
 * "posy" = posicao relativa as 'colunas' do mapa
 * ip = localizacao da proxima instrucao
 * rbp = localizacao do endereco de retorno atual
 * time = no futuro, identificacao do time do robo
 * instrSize = tamanho do vetor de instrucoes
 * prog = vetor de instrucoes
 * cristais = quantos cristais o robo esta carregando
 */
Maquina cria_maquina(INSTR *p, int iSize, int id, int posx, int posy, int vida, int time) {
    Maquina m;
    for (int i = 0; i < 100; i++) m.Mem[i] = cria_operando(NUM, 0);
    m.pil = cria_pilha();
    m.exec = cria_pilha();
    m.id = id;
    m.position[0]=posx;
    m.position[1]=posy;
    m.ip = 0;
    m.rbp = 0;
    m.time = time;
    m.cont = 0;
    m.vida = vida;
    m.instrSize = iSize;
    m.prog = malloc(iSize*sizeof(INSTR));
    memcpy(m.prog, p, iSize*sizeof(INSTR));
    m.cristais = 0;
    return m;
}

/* Alguns macros para facilitar a leitura e escrita do código */
#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)
#define rbp (m->rbp)
#define id (m->id)
#define posx (m->position[0])
#define posy (m->position[1])
#define cris (m->cristais)

// Limpa o vetor de instrucoes do robo
void destroi_maquina(Maquina m) {
    free(m.prog);
}

/*
 * Cria um novo frame, colocando
 * o rbp na pilha de retornos
 */
int new_frame(Maquina *m, int n) {
    int ibc = rbp;
    if (ibc < MAXFRAMES-1) {
        m->pilret[++rbp] = n+ibc;
        return rbp;
    }
    return -1;
}

/*
 * Deleta um frame, decrementando
 * o rbp
 */
int del_frame(Maquina *m) {
    if (rbp > 0) return --rbp;
    return -1;
}

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
 * A funcao executa a interpretacao de no maximo n
 * instrucoes. De qualquer forma, uma chamada ao sistema
 * interrompe imediatamente o funcionamento do robo.
 * Isso faz com que a arena, apos fornecer o feedback
 * de sua chamada, ceda a vez para outro robo
 */
void exec_maquina(Maquina *m, int n) {
    int i;
    OPERANDO resposta, tmpop, tmpop2;
    for (i = 0; i < n; i++) {
            if (ip>=m->instrSize) ip = 0;
            OpCode     opc = prg[ip].instr;
            OPERANDO arg = prg[ip].op;

            D(printf("Turno da máquina %d: \n[Cristais: %d, X: %d, Y: %d]\n\n", id, cris, posx, posy));
            D(printf("%3d: %-4.4s     ", ip, CODES[opc]); imprime_op(arg); puts("");); 
      
        switch (opc) {
            //Se pilha de dados tiver mais que dois elementos, desempilha ambos
            //os soma, e adiciona o resultado a pilha
            case ADD:
                if(pil->topo >= 2){
                        tmpop = desempilha(pil);
                        tmpop2 = desempilha(pil);
                        if (tmpop.t==NUM && tmpop2.t==NUM)
                            empilha(pil, cria_operando(NUM, tmpop.valor.n+tmpop2.valor.n));
                        else {
                            empilha(pil, tmpop2);
                            empilha(pil, tmpop);
                            Erro("Tipos inválidos");
                    }
                }
                break;
            //Se pilha de dados tiver mais que dois elementos, desempilha ambos
            //subtrai o topo do segundo, e adiciona o resultado a soma a pilha
            case SUB: 
                if(pil->topo >= 2){
                    tmpop = desempilha(pil);
                    tmpop2 = desempilha(pil);
                    if (tmpop.t==NUM && tmpop2.t==NUM)
                            empilha(pil, cria_operando(NUM, tmpop2.valor.n-tmpop.valor.n));
                        else {
                            empilha(pil, tmpop2);
                            empilha(pil, tmpop);
                            Erro("Tipos inválidos");
                        }
                }
                break;
            //Se pilha de dados tiver mais que dois elementos, desempilha ambos
            //Multiplica seus valores e adiciona o resultado a pilha
            case MUL: 
                if(pil->topo >= 2){
                    tmpop = desempilha(pil);
                    tmpop2 = desempilha(pil);
                    if (tmpop.t==NUM && tmpop2.t==NUM)
                        empilha(pil, cria_operando(NUM, tmpop.valor.n*tmpop2.valor.n));
                    else {
                        empilha(pil, tmpop2);
                        empilha(pil, tmpop);
                        Erro("Tipos inválidos");
                    }
                }
                break;
            //Se pilha de dados tiver mais que dois elementos, desempilha ambos
            //Divide o segundo pelo que estava no topo, e adiciona o resultado a pilha
            case DIV: 
                if(pil->topo >= 2){  
                    tmpop = desempilha(pil);
                    tmpop2 = desempilha(pil);
                    if (tmpop.t==NUM && tmpop2.t==NUM)
                        empilha(pil, cria_operando(NUM, tmpop.valor.n/tmpop2.valor.n));
                    else {
                        empilha(pil, tmpop2);
                        empilha(pil, tmpop);
                        Erro("Tipos inválidos");
                    }
                }
                break;
            //Pula para a instrucao de indice igual ao valor encontrado no operando
            case JMP:
                    ip = arg.valor.n;
                    continue;
            //Desempilha o topo da pilha de dados e, caso o valor seja diferente de zero,
            //move o ponteiro de instrucoes para o valor desempilhado
            case JIT:
                if(pil->topo == 0) break;
                if (desempilha(pil).valor.n != 0) {
                    ip = arg.valor.n;
                    continue;
                }
                break;
            //Desempilha o topo da pilha de dados e, caso o valor seja igual a zero,
            //move o ponteiro de instrucoes para o valor desempilhado
            case JIF: 
                if(pil->topo == 0) break;
                if (desempilha(pil).valor.n == 0) {
                    ip = arg.valor.n;
                    continue;
                }
                    break;
            //Empilha o endereço de retorno na pilha de execução,
            // atualiza o valor de rbp para o topo
            case CALL:
                if (arg.t==NUM) {
                    empilha(exec, cria_operando(NUM, ip));
                    ip = arg.valor.n;
                    // rbp = exec->topo;
                    continue;
                }
            //Atualiza o valor do topo para o valor de rbp e muda
            // o ip para o endereço de retorno
            case RET:
                // exec->topo = rbp;
                ip = desempilha(exec).valor.n;
                break;
            //Desempilha dois operandos da pilha de dados, caso sejam iguais,
            //empilha '1' e caso sejam diferentes, empilha '0'
            case EQ:
                if(pil->topo < 2) break;
                tmpop = desempilha(pil);
                tmpop2 = desempilha(pil);
                
                if (tmpop.valor.n == tmpop2.valor.n)
                    empilha(pil, cria_operando(NUM, 1));
                else
                    empilha(pil, cria_operando(NUM, 0));
                
                break;
            //Desempilha dois operandos da pilha de dados, caso o primeiro seja
            //menor que o segundo, empilha '1', caso contrario, empilha '0'
            case GT:
                if(pil->topo < 2) break;
                tmpop = desempilha(pil);
                tmpop2 = desempilha(pil);

                    if (tmpop.valor.n < tmpop2.valor.n)
                        empilha(pil, cria_operando(NUM, 1));
                    else
                        empilha(pil, cria_operando(NUM, 0));
                
                break;
            //Desempilha dois operandos da pilha de dados, caso o primeiro seja
            //menor ou igual ao segundo, empilha '1', caso contrario, empilha '0'
            case GE:
                if(pil->topo < 2) break;
                tmpop = desempilha(pil);
                tmpop2 = desempilha(pil);

                    if (tmpop.valor.n <= tmpop2.valor.n)
                        empilha(pil, cria_operando(NUM, 1));
                    else
                        empilha(pil, cria_operando(NUM, 0));
                
                break;
            //Desempilha dois operandos da pilha de dados, caso o primeiro seja
            //maior que o segundo, empilha '1', caso contrario, empilha '0'
            case LT:
                if(pil->topo < 2) break;
                tmpop = desempilha(pil);
                tmpop2 = desempilha(pil);
                
                    if (tmpop.valor.n > tmpop2.valor.n)
                        empilha(pil, cria_operando(NUM, 1));
                    else
                        empilha(pil, cria_operando(NUM, 0));
                
                break;
            //Desempilha dois operandos da pilha de dados, caso o primeiro seja
            //maior ou igual ao segundo, empilha '1', caso contrario, empilha '0'
            case LE:
                if(pil->topo < 2) break;
                tmpop = desempilha(pil);
                tmpop2 = desempilha(pil);
                
                    if (tmpop.valor.n >= tmpop2.valor.n)
                        empilha(pil, cria_operando(NUM, 1));
                    else
                        empilha(pil, cria_operando(NUM, 0));
                
                break;
            //Desempilha dois operandos da pilha de dados, caso sejam diferentes,
            //empilha '1' e caso sejam diferentes, empilha '0'
            case DIFF:
                if(pil->topo < 2) break;
                tmpop = desempilha(pil);
                tmpop2 = desempilha(pil);
                
                    if (tmpop.valor.n != tmpop2.valor.n)
                        empilha(pil, cria_operando(NUM, 1));
                    else
                        empilha(pil, cria_operando(NUM, 0));
                
                break;
            //Troca o sinal do topo da pilha
            case NEG:
                if(pil->topo < 1) break;
                tmpop = desempilha(pil);
                if (tmpop.t == NUM)
                    empilha(pil, cria_operando(NUM, -tmpop.valor.n));
                else
                    Erro("Operação inválida");
                break;
            // Cria um novo frame iniciando na posição arg
            case ENTRY:
                if (arg.t==NUM)
                    new_frame(m, arg.valor.n);
                break;
            // Deleta um frame  
            case LEAVE:
                del_frame(m);
                break;
            // Empilha o argumento na pilha de dados
            case PUSH:
                empilha(pil, arg);
                break;
            // Desempilha um operando da pilha de dados
            case POP:
                if(pil->topo == 0) break;
                desempilha(pil);
                break;
            // Duplica o topo da pilha
            case DUP:
                if(pil->topo == 0) break;
                tmpop = desempilha(pil);
                empilha(pil, tmpop);
                empilha(pil, tmpop);
                break;
            // Store: desempilha um operando da pilha de dados
            // e armazena na posição de memória dada por arg
            case STO:
                if(pil->topo == 0) break;
                if(arg.t!=NUM) break;
                m->Mem[arg.valor.n] = desempilha(pil);
                break;
            // Recall: empilha um operando na pilha de dados
            // localizado na memória na posição arg   
            case RCL:
                if(arg.t!=NUM) break;
                empilha(pil,m->Mem[arg.valor.n]);
                break;
            // Recall Memory: desempilha um operando da pilha de
            // dados e usa seu valor para buscar outro operando na
            // memória, empilhando-o na pilha de dados
            case RCM:
                if(pil->topo == 0) break;
                tmpop = desempilha(pil);
                if(tmpop.t!=NUM) break;
                empilha(pil, m->Mem[tmpop.valor.n]);
                break;
            // Fim do programa: robo torna-se inativo
            case END:
                return;
            // Imprime o topo da pilha
            case PRN:
                if(pil->topo == 0) break;
                imprime_op(desempilha(pil));
                puts("\n");
                break;
                
            //Faz uma chamada a arena
            //Algumas instrucoes precisam de informacoes que apenas
            //a arena pode oferecer. Nesses caso, e necessario uma
            //chamada ao sistema. Porem, ao chamar o sistema, o robo
            //perde a vez
            case SYS: 
                resposta = cria_operando(NUM, Sistema(id));
                m->Mem[8] = resposta;
                i=n;
                break;
            //Desempilha um tile e empilha a informacao desejada sobre ele
            //na pilha de dados
            case ATR:
                if(pil->topo == 0) break;
                tmpop = desempilha(pil);
                if(tmpop.t==TILE){
                    switch (arg.valor.n){
                        case 0:
                            empilha(pil, cria_operando(TERRENO, tmpop.valor.tile.terreno));
                            break;
                        case 1:
                            empilha(pil, cria_operando(NUM, tmpop.valor.tile.cristais));
                            break;
                        case 2:
                            empilha(pil, cria_operando(NUM, tmpop.valor.tile.ocupado));
                            break;
                        case 3:
                            empilha(pil, cria_operando(NUM, tmpop.valor.tile.base));
                            break;
                        default:
                            Erro("Atributo inexistente");
                            break;
                    }
                }
                else
                    Erro("Tipo invalido");
                break;
            }
    
        //Imprimindo as pilhas e a memória por motivos de debug
        D(puts("\nPilha de dados:\n"));
        D(imprime(pil,10));
        D(puts("\n\n Pilha de execução: \n"));
        D(imprime(exec,10));
        D(puts("\n"));
        D(puts("\nMemória:\n"));
        D(imprime_mem(m));
        D(puts("\n"));
        D(puts("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"));
        ip++;
    }
}
    
//Funções auxiliares

/*
 * Retorna um operando tendo como base o tipo e o numero 
 * passados como argumentos
 */
OPERANDO cria_operando(Tipo t, int arg){
    OPERANDO a;
    a.t = t;
    switch(t){
    case NUM:
    a.valor.n = arg;
        break;
    case ACAO:
        a.valor.ac = arg;
        break;
    case VAR:
        a.valor.v = arg;
        break;
    case TERRENO:
        a.valor.terr = arg;
        break;
    default: ;
    }
    return a;
}

/*
 * Retorna uma instrucao tendo como base o opcode e o OPERANDO 
 * passados como argumentos
 */
INSTR cria_instr(OpCode opc, OPERANDO arg){
    INSTR comando;
    comando.instr = opc;
    comando.op = arg;
    return comando;
}

/*
 * Imprime o valor numerico do operando de uma
 * maneira legivel
 */
void imprime_op(OPERANDO arg){
    switch(arg.t){
    case NUM:
        printf("%d", arg.valor.n);
        break;
    case ACAO:
        printf("%s", Chamadas[arg.valor.ac]);
        break;
    case VAR:
        printf("%s", Direcao[arg.valor.v]);
        break;
    case TILE:
        printf("Tile {%s, %d, %d, %d}", Terrenos[arg.valor.tile.terreno], arg.valor.tile.cristais, arg.valor.tile.ocupado, arg.valor.tile.base);
        break;
    case TERRENO:
        printf("Terreno {%s}", Terrenos[arg.valor.terr]);
    default:
        break;
    }
}

/*
 * Funcao auxiliar que imprime as primeiras
 * 15 posicoes da memoria do robo
 */
void imprime_mem(Maquina *m){
    printf("[");
    for (int i = 0; i <= 15; i++){ 
    printf(" ");
    imprime_op(m->Mem[i]);
    printf(", ");
    }
    printf("]");
    return;
}
