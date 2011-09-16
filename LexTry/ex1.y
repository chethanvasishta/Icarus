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
%nonassoc '(' ')'


%token<val> INTEGER NUMBER FLOAT VOID RETURN
%token<name> IDENTIFIER

%%
program: program statement
	| program func_decl
	| program func_defn
	|
	;

func_decl: datatype IDENTIFIER '(' arglist ')' ';' { trace("function declaration "); }
	;
arglist: datatype IDENTIFIER
	| arglist ',' datatype IDENTIFIER
	|
	;

func_defn: datatype IDENTIFIER '(' arglist ')' '{' statement_block '}' {trace("function definition "); } ;

statement_block: statement_block statement |  ;
	
statement: declaration 
	| assignment
	| func_call
	| return_stmt
	| ';' {trace("statement ");}
	;
declaration: datatype varList ';'	{ trace("declaration ");}
varList: IDENTIFIER	{ symTab.push_back($1); }
	| varList',' IDENTIFIER { symTab.push_back($3); }
	;
datatype: INTEGER 	{ trace("int "); }
	| FLOAT 	{ trace("float "); }
	| VOID		{ trace("void "); }
	;
assignment: IDENTIFIER '=' expression ';'	{ trace("assignment"); } // need to search a better name for assignment. This is a statement
return_stmt: RETURN expression;
expression: NUMBER 
	| IDENTIFIER 
	| expression '+' expression 
	| expression '-' expression 
	| expression '*' expression 
	| expression '/' expression 
	| func_call
	| '('expression')' 
	;
func_call: IDENTIFIER'('paramlist')'
paramlist: expression
	| paramlist',' expression
	|
	;
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

int ParseFile(char *filename){
	FILE *fp = fopen(filename, "r");
	if(fp == NULL) return -1;
	yyin = fp;
	yyparse();
	return 0;	
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
