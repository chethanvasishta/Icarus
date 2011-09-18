all: main parser codegen
	g++ main.o lex.yy.o parser.o codegen.o -o Icarus
main: main.cpp
	g++ -c main.cpp -o main.o
parser:
	yacc -d parser.y -o y.tab.cc && lex lexer.l && cc -c lex.yy.c -o lex.yy.o && g++ -c y.tab.cc -o parser.o
codegen:
	g++ -c codegen.cpp -o codegen.o
clean:
	rm -rf *.o y.tab.cc y.tab.hh
