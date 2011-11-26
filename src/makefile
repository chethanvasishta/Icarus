CC = g++

all: lexer parser main
	$(CC) obj/*.o -lfl -o Icarus
parser:
	yacc -d parser.y -o y.tab.cc && $(CC) -c y.tab.cc -o parser.o && rm y.tab.cc
lexer: parser
	flex -+ lexer.l && $(CC) -c lex.yy.cc -o lexer.o && rm lex.yy.cc y.tab.hh
main:
	find -name '*.cpp' -print0 | xargs -0 $(CC) -c && mkdir -p obj && mv *.o ./obj/
#debug

debug: lexerd parserd maind
	$(CC) obj/*.o -lfl -o IcarusD
maind: 
	find -name '*.cpp' -print0 | xargs -0 $(CC) -g -c && mkdir -p obj && mv *.o ./obj/
parserd:
	yacc -d parser.y -o y.tab.cc && $(CC) -g -c y.tab.cc -o parser.o
lexerd: parserd
	flex -+ lexer.l && $(CC) -g -c lex.yy.cc -o lexer.o
#clean
clean:
	rm -rf obj y.tab.cc y.tab.hh lex.yy.cc Icarus IcarusD *.dot
