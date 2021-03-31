# Simple Makefile

CC=/usr/bin/cc

<<<<<<< Updated upstream
all:  flex-config bison-config nutshparser nutshscanner nutshell nutshell-out

flex-config:
	flex nutflex.l

bison-config:
	bison -d nutshparser.y
=======
all: nutshell lex.yy.o nutshparser.tab.o

lex.yy.o: nutflex.l 
	flex nutflex.l
	$(cc) -c lex.yy.c

nutshparser.tab.o: nutshparser.y
	bison -d nutshparser.y
	$(CC) -c nutshparser.tab.c
>>>>>>> Stashed changes

nutshscanner:  lex.yy.c
	$(CC) -c lex.yy.c -o nutshscanner.lex.o

<<<<<<< Updated upstream
nutshparser:  nutshparser.tab.c 
	$(CC) -c nutshparser.tab.c -o nutshparser.y.o

nutshell:  nutshell.c
	$(CC) -g -c nutshell.c -o nutshell.o 

nutshell-out: 
	$(CC) -o nutshell nutshell.o nutshscanner.lex.o nutshparser.y.o -ll -lm -lfl
=======
nutshell: nutshparser.tab.o lex.yy.o nutshell.o
	$(CC) -o nutshell lex.yy.o nutshparser.tab.o nutshell.o -ll

clean:
	rm -f lex.yy.c nutshparser.tab.c nutshparser.tab.h nutshell *.o
>>>>>>> Stashed changes

clean:
	rm -f nutshell nutshell.o nutshscanner.lex.o nutshparser.y.o nutshparser.tab.c nutshparser.tab.h lex.yy.c y.tab.c y.tab.h