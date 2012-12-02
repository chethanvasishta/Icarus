CC = g++

LLVM_MODULES = core jit native

CPPFLAGS = `llvm-config --cppflags $(LLVM_MODULES)` -I./include
LDFLAGS = `llvm-config --ldflags $(LLVM_MODULES)`
LIBS = `llvm-config --libs $(LLVM_MODULES)`

all: lexer parser main
	$(CC) obj/*.o $(LIBS) $(LDFLAGS) -lfl -o Icarus
parser:
	yacc -d -t parser.y -o y.tab.cc && $(CC) -c $(CPPFLAGS) y.tab.cc -o parser.o && rm y.tab.cc
lexer: parser
	flex -+ lexer.l && $(CC) -c  $(CPPFLAGS) lex.yy.cc -o lexer.o && rm lex.yy.cc y.tab.hh
main:
	find -name '*.cpp' -print0 | xargs -0 $(CC) -c $(CPPFLAGS) && mkdir -p obj && mv *.o ./obj/
#debug

debug: lexerd parserd maind
	$(CC) obj/*.o $(LIBS) $(LDFLAGS) -lfl -o IcarusD
maind: 
	find -name '*.cpp' -print0 | xargs -0 $(CC) -g -c $(CPPFLAGS) && mkdir -p obj && mv *.o ./obj/
parserd:
	yacc -d -v -t -g parser.y -o y.tab.cc && $(CC) -g -c $(CPPFLAGS) y.tab.cc -o parser.o
lexerd: parserd
	flex -+ lexer.l && $(CC) -g -c $(CPPFLAGS) lex.yy.cc -o lexer.o
#clean
clean:
	rm -rf obj y.tab.cc y.tab.hh lex.yy.cc Icarus IcarusD *.dot temp.* a.out
