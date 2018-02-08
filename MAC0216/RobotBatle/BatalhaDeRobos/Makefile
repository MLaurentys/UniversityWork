CFLAGS = -Wall --std=c11 -g
CC=gcc

.PHONY: clean


all: arena


arena: arena.o maq.o pilha.o compila.tab.o lex.yy.o symrec.o acertos.o -lfl
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


compila: compila.tab.o lex.yy.o symrec.o acertos.o maq.o arena.o pilha.o
	gcc -o $@ $^ -lm -lfl

compila.tab.o: compila.y
	bison -d compila.y
	gcc -c -g compila.tab.c

lex.yy.o: compila.l
	flex compila.l
	gcc -c -g lex.yy.c


clean:
	rm -f *.o lex.yy.c compila.tab.c compila.tab.h *~ compila


