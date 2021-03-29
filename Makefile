
#Use GNU compiler
cc = gcc -g
CC = g++ -g

all: nutshell

lex.yy.o: nutflex.l 
	lex nutflex.l
	$(cc) -c lex.yy.c

y.tab.o: nutshparser.y
	yacc -d nutshparser.y
	$(CC) -c y.tab.c

nutshell.o: nutshell.cpp
	$(CC) -c nutshell.cpp

nutshell: y.tab.o lex.yy.o nutshell.o
	$(CC) -o nutshell lex.yy.o y.tab.o nutshell.o -ll

clean:
	rm -f lex.yy.c y.tab.c y.tab.h nutshell *.o

