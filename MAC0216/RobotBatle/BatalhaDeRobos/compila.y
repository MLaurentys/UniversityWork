/* Compilador */

%{
#include <stdio.h>
#include <math.h>
#include "symrec.h"
#include "acertos.h"
#include "instr.h"

// Declaracoes do lex:  
int yylex();
int compilador();
void yyerror(char const *);
void Fatal();

// Variaveis principais
static int ip  = 0;					// ponteiro de instrucoes 
static int mem = 9;				// ponteiro da memoria 
static INSTR *prog;					// programa
static int parmcnt = 0;		// contador de parametros 

// Variaveis axiliares para armazenar a ultima expressao
// de um "for"
static int ip2 = 0;
static INSTR prog2[10];

// Funcoes auxiliares
void AddInstr(OpCode op, OPERANDO valor) {
  	prog[ip++] = (INSTR) {op,  valor};
}

void AddInstr2(OpCode op, OPERANDO valor){
	prog2[ip2++] = (INSTR) {op, valor};
}
%}

/*  Declaracoes */
%union {
	int num;
	double val;
	Dir direc;
	Terreno terr;
	char cod[30];
}

%token <num> ATRt
%token <val>  NUMt
%token <cod> ID
%token <direc> DIRECAOt
%token <terr> TERRt
%token ADDt SUBt MULt DIVt ASGN OPEN CLOSE RETt EOL
%token ADDs SUBs MULs DIVs
%token EQt NEt LTt LEt GTt GEt INCt DECt ABRE FECHA SEP PONTO NEW TRUEt RESPt
%token IF ELSE WHILE FOR FUNC PRINT VERt MOVt RECt DEPt ATQt JGCt KMKt VERt2

%right ASGN
%left ADDt SUBt
%left MULt DIVt
%left NEGt
%left LTt GTt LEt GEt EQt NEt

%%

/* Gramatica */

// Programa: sequencia de comandos
Programa: Comando
       | Programa Comando
;

// Comando: expressao, condicional, laço, print, return ou chama de sistema
Comando: Expr EOL
       | Cond
       | Loop
       | Func
	   | PRINT Expr EOL { AddInstr(PRN, (OPERANDO) {NUM, 0}); }
	   | Return
 	   | Sis EOL
;

// Return: Pode retornar um valor ou nao (com ou sem parenteses)
Return:  RETt EOL {
		 	AddInstr(LEAVE, (OPERANDO) {NUM, 0});
			AddInstr(RET, (OPERANDO) {NUM, 0});
 			}
 		| RETt Expr EOL {
		 	   AddInstr(LEAVE, (OPERANDO) {NUM, 0});
			   AddInstr(RET, (OPERANDO) {NUM, 0});
 			}
;

// Expressao condicional: expressao seguida de <, >, <=, >=, == ou !=
// seguido de outra expressão, ou a keyword "True/true"
ExprC:  Expr LTt Expr  { AddInstr(LT,   (OPERANDO) {NUM, 0});}
	| Expr GTt Expr  { AddInstr(GT,   (OPERANDO) {NUM, 0});}
	| Expr LEt Expr  { AddInstr(LE,   (OPERANDO) {NUM, 0});}
	| Expr GEt Expr  { AddInstr(GE,   (OPERANDO) {NUM, 0});}
	| Expr EQt Expr  { AddInstr(EQ,   (OPERANDO) {NUM, 0});}
	| Expr NEt Expr  { AddInstr(NE,   (OPERANDO) {NUM, 0});}
	| TRUEt { AddInstr(PUSH, (OPERANDO) {NUM, 1});}
;

// Expressao: tipo mais complexo, pode ser:
// Um numero
// Uma variavel
// Uma atribuicao
// Declaracao de variavel: new ID - forca a criacao de uma nova variavel
// Uma checagem de atributo: ID.atributo
// Um ver, uma chamada de sistema
// Expressao (+-*/) outra expressao
// ID++ ou ID-- (incrementa e decrementa)
// ID(+-*/)= expressao
Expr: NUMt {  AddInstr(PUSH, (OPERANDO) {NUM, $1});}
	| NEW ID ASGN Expr {
						symrec *s = putsym($2); 
						AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
						}
    | ID   {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); 
			AddInstr(RCL, (OPERANDO) {NUM, mem + s->val});
	       }
	| ID ASGN Expr {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); 
			AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
 		 	}
 	| Resposta	 
 	| Direcao	
 	| Checa_dir
 	| Terreno
	| ID PONTO ATRt  { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr(ATR, (OPERANDO) {NUM, $3}); 
  	 		} 
	| Checa_dir PONTO ATRt { AddInstr(ATR, (OPERANDO) {NUM, $3}); } 
	| Chamada 
    | Expr ADDt Expr { AddInstr(ADD,  (OPERANDO) {NUM, 0});}
	| Expr SUBt Expr { AddInstr(SUB,  (OPERANDO) {NUM, 0});}
	| Expr MULt Expr { AddInstr(MUL,  (OPERANDO) {NUM, 0});}
	| Expr DIVt Expr { AddInstr(DIV,  (OPERANDO) {NUM, 0});}
    | SUBt Expr  { AddInstr(NEG,  (OPERANDO) {NUM, 0}); }
	| OPEN Expr CLOSE
	| ID INCt {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); /* não definida */
			AddInstr(RCL, (OPERANDO) {NUM, mem + s->val});
			AddInstr(PUSH, (OPERANDO) {NUM, 1});
			AddInstr(ADD, (OPERANDO) {NUM, 0});
			AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
 		 	}
 	| ID DECt {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); /* não definida */
			AddInstr(RCL, (OPERANDO) {NUM, mem + s->val});
			AddInstr(PUSH, (OPERANDO) {NUM, 1});
			AddInstr(SUB, (OPERANDO) {NUM, 0});
			AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
 		 	}
 	| ID ADDs Expr { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr(ADD, (OPERANDO) {NUM, 0});
	  		AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
  	 		} 
 	| ID SUBs Expr { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  	 	AddInstr(SUB, (OPERANDO) {NUM, 0});
	  		AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
  	 		} 
 	| ID MULs Expr { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr(MUL, (OPERANDO) {NUM, 0});
	  		AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
  	 		}
 	| ID DIVs Expr { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr(DIV, (OPERANDO) {NUM, 0});
	  		AddInstr(STO, (OPERANDO) {NUM, mem + s->val});
  	 		} 
;

// Expressão auxiliar: armazena as instruções no vetor de 
// programas secundário: serão copiadas para o vetor principal
// após o término do bloco do "for"
// Basicamente uma copia das regras anteriores
ExprM: NUMt {  AddInstr2(PUSH, (OPERANDO) {NUM, $1});}
    | ID   {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); 
			AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val});
	       }
	| ID ASGN ExprM {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); 
			AddInstr2(STO, (OPERANDO) {NUM, mem + s->val});
 		 	}
 	| Checa_dir
	| ID PONTO ATRt  { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  	 	AddInstr2(ATR, (OPERANDO) {NUM, $3}); 
  	 		} 
	| Chamada 
    | ExprM ADDt ExprM { AddInstr2(ADD,  (OPERANDO) {NUM, 0});}
	| ExprM SUBt ExprM { AddInstr2(SUB,  (OPERANDO) {NUM, 0});}
	| ExprM MULt ExprM { AddInstr2(MUL,  (OPERANDO) {NUM, 0});}
	| ExprM DIVt ExprM { AddInstr2(DIV,  (OPERANDO) {NUM, 0});}
	| SUBt Expr  { AddInstr(NEG,  (OPERANDO) {NUM, 0}); }
	| OPEN ExprM CLOSE
	| ID INCt {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); /* não definida */
			AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val});
			AddInstr2(PUSH, (OPERANDO) {NUM, 1});
			AddInstr2(ADD, (OPERANDO) {NUM, 0});
			AddInstr2(STO, (OPERANDO) {NUM, mem + s->val});
 		 	}
 	| ID DECt {
	        symrec *s = getsym($1);
			if (s==0) s = putsym($1); /* não definida */
			AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val});
			AddInstr2(PUSH, (OPERANDO) {NUM, 1});
			AddInstr2(SUB, (OPERANDO) {NUM, 0});
			AddInstr2(STO, (OPERANDO) {NUM, mem + s->val});
 		 	}
 	 | ID ADDs ExprM { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr2(ADD, (OPERANDO) {NUM, 0});
	  		AddInstr2(STO, (OPERANDO) {NUM, mem + s->val});
  	 		} 
 	| ID SUBs ExprM { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr2(SUB, (OPERANDO) {NUM, 0});
	  		AddInstr2(STO, (OPERANDO) {NUM, mem + s->val});
  	 		} 
 	| ID MULs ExprM { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr2(MUL, (OPERANDO) {NUM, 0});
	  		AddInstr2(STO, (OPERANDO) {NUM, mem + s->val});
  	 		}
 	| ID DIVs ExprM { 
	        symrec *s = getsym($1); 
	  		if (s==0) s = putsym($1);  
	  		AddInstr2(RCL, (OPERANDO) {NUM, mem + s->val}); 
	  		AddInstr2(DIV, (OPERANDO) {NUM, 0});
	  		AddInstr2(STO, (OPERANDO) {NUM, mem + s->val});
  	 		}
; 

// Condicionais: if (expressao condicional) sozinho ou if seguido de um else
Cond: IF OPEN {
				AddInstr(PUSH, (OPERANDO) {NUM, 0});} ExprC {
  	  	 	   	salva_end(ip);
			   	AddInstr(JIF,  (OPERANDO) {NUM, 0});
 		 		}
		CLOSE  Bloco {
		 		AddInstr(PUSH, (OPERANDO) {NUM, 1});
		   		prog[pega_end()].op.valor.n = ip;
		 		}
		| Cond ELSE {
				salva_end(ip); 
				AddInstr(JIT, (OPERANDO) {NUM, 0}); 
				}
				Bloco { prog[pega_end()].op.valor.n = ip; }
;

// Loops: while (expressao condicional) ou 
// for (expressao, expressao condicional, expressao)
// Nota: para loops infinitos, while(true) é uma construcao valida
// (e recomendada, dado que o robo fica inutil apos concluir seu programa)
Loop: WHILE OPEN  { salva_end(ip); }
	  ExprC  {
	  		salva_end(ip); 
	  		AddInstr(JIF, (OPERANDO) {NUM, 0});
	  		}
	  CLOSE Bloco {
			int tmp = pega_end();
			AddInstr(JMP, (OPERANDO) {NUM, pega_end()});
			prog[tmp].op.valor.n = ip;
			} 
	| FOR OPEN Expr EOL { salva_end(ip); } 
		ExprC EOL{ 	
			salva_end(ip); 
			AddInstr(JIF, (OPERANDO) {NUM, 0});
			}
		ExprM { 	
		 	salva_end(ip2); 
		 	while(ip2 > 0)
		 		push(prog2[--ip2]);
		 	}
		CLOSE Bloco  { 
		 	int a = pega_end();
		 	for (int i = 0; i < a; i++) 
		 		prog[ip++] = pop();
			int tmp = pega_end();
			AddInstr(JMP, (OPERANDO) {NUM, pega_end()});
			prog[tmp].op.valor.n = ip;
			}
;

// Chamadas de sistema: mover, recolher cristal, depositar cristal,
// atacar, jogar cristal ou kamikaze - na pratica, são como funcoes
// pre-definidas que recebem como argumento uma direcao (ou nenhum
// argumento no caso do kamikaze)
Sis: Mov
	| Rec
	| Dep
	| Atq
	| Jgc
	| Kmk
	| Ver

// Uso: move(direcao) - Ex: move(NW)
Mov: MOVt OPEN  Direcao CLOSE { 
								   AddInstr(PUSH, (OPERANDO) {ACAO, MOV});
								   AddInstr(SYS, (OPERANDO) {NUM, 0});
								}

// Uso: recolhe(direcao) - Ex: recolhe(S)								
Rec: RECt OPEN  Direcao CLOSE { 
								   AddInstr(PUSH, (OPERANDO) {ACAO, REC});
								   AddInstr(SYS, (OPERANDO) {NUM, 0});
								}

// Uso: deposita(direcao) - Ex: deposita(CN)								
Dep: DEPt OPEN  Direcao CLOSE { 
								   AddInstr(PUSH, (OPERANDO) {ACAO, DEP});
								   AddInstr(SYS, (OPERANDO) {NUM, 0});
								}	

// Uso: ataca(direcao) - Ex: ataca(N)
Atq: ATQt OPEN  Direcao CLOSE { 
								   AddInstr(PUSH, (OPERANDO) {ACAO, ATQ});
								   AddInstr(SYS, (OPERANDO) {NUM, 0});
								}	
// Uso: joga_cristal(direcao) - Ex: joga_cristal(NE)														
Jgc: JGCt OPEN  Direcao CLOSE { 
								   AddInstr(PUSH, (OPERANDO) {ACAO, JGC});
								   AddInstr(SYS, (OPERANDO) {NUM, 0});
								}

// Uso: ver()
Ver: VERt OPEN CLOSE {
								   AddInstr(PUSH, (OPERANDO) {ACAO, VER});
								   AddInstr(SYS, (OPERANDO) {NUM, 0});
									}

// Uso: checa(direcao)
Checa_dir: VERt2 OPEN Direcao_esp CLOSE { AddInstr(RCM, (OPERANDO) {NUM, 0});}

// Uso: kamikaze()
Kmk: KMKt OPEN  CLOSE { 
						AddInstr(PUSH, (OPERANDO) {ACAO, KMK});
						AddInstr(SYS, (OPERANDO) {NUM, 0});
						}

// Empilha um operando do tipo VAR (direcao)
Direcao: DIRECAOt { AddInstr(PUSH, (OPERANDO) {VAR, $1}); }
		 | ID

// Empilha um operando do tipo NUM que representa uma direcao:
// usada em conjunto com o RCM 
Direcao_esp: DIRECAOt { AddInstr(PUSH, (OPERANDO) {NUM, $1}); }
			| ID

Resposta: RESPt OPEN CLOSE {AddInstr(RCL, (OPERANDO) {NUM, 8}); }

// Bloco: { comandos }
Bloco: ABRE Comandos FECHA ;

// Terrenos:
Terreno: TERRt {AddInstr(PUSH, (OPERANDO) {TERRENO, $1}); }

// Comandos: Sequência de comando(s)
Comandos: Comando 
    | Comandos Comando
	;

// Declaração de função: def (argumentos) ou func (argumentos)
Func: FUNC ID {
		salva_end(ip);
		AddInstr(JMP,  (OPERANDO) {NUM, 0});
		symrec *s = getsym($2);
		if (s==0) s = putsym($2);
		else {
			yyerror("Função definida duas vezes\n");
			YYABORT;
		}
		s->val = ip;
	  	} OPEN { newtab(lastval()); }
	  	Args CLOSE  Bloco {
			AddInstr(LEAVE, (OPERANDO) {NUM, 0});
			AddInstr(RET, (OPERANDO) {NUM, 0});
			prog[pega_end()].op.valor.n = ip;
			deltab();
	  	}
;

// Argumentos: identificadores separados por ","
Args: 
	| ID { putsym($1); }
    | Args SEP ID { putsym($3); }
;

// Chamada de função:
Chamada: ID OPEN { parmcnt = mem; }
		ListParms{
			symrec *s = getsym($1);
			if (s == 0) {
				yyerror("Função não definida\n");
				YYABORT;
		   	}
		   	AddInstr(ENTRY, (OPERANDO) {NUM, lastval()});
		   	/* Cópia dos parâmetros */
		   	while (parmcnt > mem) 
			 	AddInstr( STO, (OPERANDO) {NUM, --parmcnt});
		   	AddInstr(CALL, (OPERANDO) {NUM, s->val});
		 	}
	  	CLOSE 
;

ListParms: 
	| Expr { parmcnt++;}
	| Expr { parmcnt++;} SEP ListParms
;

%%
extern FILE *yyin;

// Mensagem de erro 
void yyerror(char const *msg) {
	fprintf(stderr, "IP: %d\n", ip);
	fprintf(stderr,"ERRO: %s\n",msg);
}

// Funcao que chama o yyparse para compilar o programa
// que se encontra no arquivo dado como argumento
int compilador(FILE *cod, INSTR *dest, int id) {
	ip = 0;
	cleartab();
	yyin = cod;
	prog = dest;
	int r = yyparse();
	if (r == 0){
		AddInstr(END,(OPERANDO) {NUM, 0});
		return ip;
	}
	else Fatal("Erro na compilacao: cheque os seus programas!");
}
