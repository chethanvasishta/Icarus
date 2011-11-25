CC = g++

all: lexer parser main
	$(CC) *.o -lfl -o Icarus
parser:
	yacc -d parser.y -o y.tab.cc && $(CC) -c y.tab.cc -o parser.o
lexer: parser
	flex -+ lexer.l && $(CC) -c lex.yy.cc -o lexer.o
main:
	$(CC) -c *.cpp
#debug

debug: lexerd parserd maind
	$(CC) *.o -lfl -o IcarusD
maind: 
	$(CC) -g -c *.cpp
parserd:
	yacc -d parser.y -o y.tab.cc && $(CC) -g -c y.tab.cc -o parser.o
lexerd: parserd
	flex -+ lexer.l && $(CC) -g -c lex.yy.cc -o lexer.o
#clean
clean:
	rm -rf *.o y.tab.cc y.tab.hh lex.yy.cc Icarus IcarusD *.dot
