
#Use GNU compiler
cc = gcc -g
CC = g++ -g

all: nutshell lex.yy.o nutshparser.tab.o

lex.yy.o: nutflex.l 
	flex nutflex.l
	$(cc) -c lex.yy.c

nutshparser.tab.o: nutshparser.y
	bison -d nutshparser.y
	$(CC) -c nutshparser.tab.c

nutshell.o: nutshell.cpp
	$(CC) -c nutshell.cpp

nutshell: nutshparser.tab.o lex.yy.o nutshell.o
	$(CC) -o nutshell lex.yy.o nutshparser.tab.o nutshell.o -ll

clean:
	rm -f lex.yy.c nutshparser.tab.c nutshparser.tab.h nutshell *.o

# clean:
# 	rm -f nutshell nutshell.o nutshscanner.lex.o nutshparser.y.o nutshparser.tab.c nutshparser.tab.h lex.yy.c y.tab.c y.tab.h
