%{
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "codegen.h"
using namespace std;

extern "C" {
	int yylex(void);
	void yyerror(string);
}

extern FILE * yyin;

//debug definitions

static void trace(string,int);
#define DEBUG 1
#if DEBUG
static int debugLevel = 3;
#endif 

#define trace1(s) trace(s, 1)
#define trace2(s) trace(s, 2)

//globals
static Module& module = *new Module("globalModule");
%}

%union
{
    char*	name;
    int 	val;
}


%left '+' '-'
%left  '*' '/'
%nonassoc '(' ')'


%token<val> INTEGER NUMBER FLOAT VOID RETURN
%token<name> IDENTIFIER

%start program

%%
program: program statement
	| program func_decl
	| program func_defn
	|
	;

func_decl: datatype IDENTIFIER '(' arglist ')' ';' { trace2("function declaration "); }
	;
arglist: datatype IDENTIFIER
	| arglist ',' datatype IDENTIFIER
	|
	;

func_defn: datatype IDENTIFIER '(' arglist ')' '{' statement_block '}' 
	{
		trace2("function definition ");
		const std::string& name = $2;
		FunctionProtoType& fp = *new FunctionProtoType();
		Function& f = *new Function(name, fp);
		module.addFunction(f);
	} ;

statement_block: statement_block statement |  ;
	
statement: declaration 
	| assignment
	| func_call
	| return_stmt
	| ';' {trace2("statement ");}
	;
declaration: datatype varList ';'	{ trace2("declaration ");}
varList: IDENTIFIER	{ module.addSymbol(*new Symbol(*new std::string($1))); }
	| varList',' IDENTIFIER { module.addSymbol(*new Symbol(*new std::string($3))); }
	;
datatype: INTEGER 	{ trace1("int "); }
	| FLOAT 	{ trace1("float "); }
	| VOID		{ trace1("void "); }
	;
assignment: IDENTIFIER '=' expression ';'	{ trace1("assignment"); } // need to search a better name for assignment. This is a statement
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
func_call: IDENTIFIER'('paramlist')' { trace2("function called"); }
paramlist: expression
	| paramlist',' expression
	|
	;
%%

extern "C" {
	void yyerror(string s) {
	    fprintf(stderr, "%s\n", s.c_str());
	}

	int yywrap (void ) {
		return 1;
	}
}


static void trace(string s, int level){
#if DEBUG
	if(level >= debugLevel)
		printf("%s",s.c_str());
#endif
}

Module* ParseFile(char *filename){
	FILE *fp = fopen(filename, "r");
	if(fp == NULL) return NULL;
	yyin = fp;
	yyparse();
	return &module;	
}
