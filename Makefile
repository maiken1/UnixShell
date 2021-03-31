# Simple Makefile

CC=/usr/bin/cc

all: nutshell lex.yy.o nutshparser.tab.o

lex.yy.o: nutflex.l 
	flex nutflex.l
	$(cc) -c lex.yy.c

nutshparser.tab.o: nutshparser.y
	bison -d nutshparser.y
	$(CC) -c nutshparser.tab.c

nutshscanner:  lex.yy.c
	$(CC) -c lex.yy.c -o nutshscanner.lex.o

nutshell: nutshparser.tab.o lex.yy.o nutshell.o
	$(CC) -o nutshell lex.yy.o nutshparser.tab.o nutshell.o -ll

clean:
	rm -f lex.yy.c nutshparser.tab.c nutshparser.tab.h nutshell *.o

# clean:
# 	rm -f nutshell nutshell.o nutshscanner.lex.o nutshparser.y.o nutshparser.tab.c nutshparser.tab.h lex.yy.c y.tab.c y.tab.h