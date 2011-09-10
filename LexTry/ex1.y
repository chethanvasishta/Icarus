%{
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

extern "C" {
	int yylex(void);
	void yyerror(string);
}
extern FILE * yyin;
static void trace(string);
#define DEBUG 1

//globals
std::vector<string> symTab;
%}

%union
{
    char* 	name;
    int 	val;
}

%left '+' '-'
%left  '*' '/'



%token<val> INTEGER NUMBER FLOAT 
%token<name> VARIABLE

%%
program: program statement
	| 
	;
statement: declaration 
	| assignment 
	| ';'
declaration: datatype decl ';'
	{ 
		trace("declaration");
		symTab.push_back($2);
	}
decl: VARIABLE
	| VARIABLE = NUMBER;
	| 
	;
datatype: INTEGER { trace("int"); }
	| FLOAT { trace("float"); }
	;
assignment: VARIABLE '=' expression ';'	
	{
		trace("assignment"); // need to search a better name for assignment. This is a statement
	}
expression: NUMBER | VARIABLE | expression '+' expression | expression '-' expression | expression '*' expression | expression '/' expression | '('expression')';
%%
extern "C" {
	void yyerror(string s) {
	    fprintf(stderr, "%s\n", s.c_str());
	}
}

static void printAllSymbols(){
#if DEBUG
	int i = 0;
	for(; i < symTab.size() ; ++i){
		printf("%4d\t:%s\n", i, symTab[i].c_str());
	}
#endif
}

static void trace(string s){
#if DEBUG
	printf("%s",s.c_str());
#endif
}

int main(int argc, char *argv[]){
	if(argc >= 2){
		FILE *fp = fopen(argv[1], "r");
		if(fp == NULL) return 1;
		yyin = fp;
#if DEBUG
		printf("setting yyin to input file %s\n", argv[1]);
#endif
	}
	yyparse();
	printAllSymbols();	
	return 0;
}

