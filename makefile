all: main lexer parser codegen errorhandler dotwriter
	g++ main.o parser.o lex.yy.o codegen.o errorhandler.o dotwriter.o -lfl -o Icarus
main: main.cpp
	g++ -c main.cpp -o main.o
parser:
	yacc -d parser.y -o y.tab.cc && g++ -c y.tab.cc -o parser.o
lexer: parser
	flex -+ lexer.l && g++ -c lex.yy.cc -o lex.yy.o
codegen:
	g++ -c codegen.cpp -o codegen.o
errorhandler:
	g++ -c icerr.cpp -o errorhandler.o
dotwriter:
	g++ -c ./Dot/dotwriter.cpp -o dotwriter.o
#debug
debug: maind lexerd parserd codegend errorhandlerd
	g++ -g main.o parser.o lex.yy.o codegen.o errorhandler.o -lfl -o IcarusD
maind: main.cpp
	g++ -g -c main.cpp -o main.o
parserd:
	yacc -d parser.y -o y.tab.cc && g++ -g -c y.tab.cc -o parser.o
lexerd: parserd
	flex -+ lexer.l && g++ -g -c lex.yy.cc -o lex.yy.o
codegend:
	g++ -g -c codegen.cpp -o codegen.o
errorhandlerd:
	g++ -g -c icerr.cpp -o errorhandler.o
clean:
	rm -rf *.o y.tab.cc y.tab.hh lex.yy.cc Icarus IcarusD
