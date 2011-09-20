%{
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "codegen.h"
#include <FlexLexer.h>
#include <fstream>
using namespace std;

int yylex(void);
extern void yyerror(string);


extern "C" FILE *yyin;
static void addSymbol(char *s);

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
static std::list<int> dataTypeList; //to store the data types of the arguments while constructing the argList. See if we can add it to a class, say Parser. I don't like globals
yyFlexLexer lexer; //this is our lexer
%}

%union
{
    char*	name;
    int 	val;
    Value*	value;
}


%left '+' '-'
%left  '*' '/'
%nonassoc '(' ')'

%token<val> INTEGER NUMBER FLOAT VOID RETURN
%token<name> IDENTIFIER

%type<value> expression

%start program

%%
program: program statement
	| program func_decl
	| program func_defn
	|
	;

func_decl: datatype IDENTIFIER '(' arglist ')' ';' 
	{ 
		trace2("function declaration ");
		const std::string& name = $2;
		FunctionProtoType& fp = *new FunctionProtoType(name, dataTypeList, $<val>1);
		dataTypeList.clear();
		IcErr err = module.addProtoType(fp);
		if(err != eNoErr)
			yyerror(errMsg[err]);
		
	}
	;
arglist: datatype IDENTIFIER { dataTypeList.push_back($<val>1); }
	| arglist ',' datatype IDENTIFIER { dataTypeList.push_back($<val>3); }
	|
	;
//as and when we find the identifiers , we need to add them to a list and use them while constructing the prototype/func

func_defn: datatype IDENTIFIER '(' arglist ')' '{' statement_block '}' 
	{
		trace2("function definition ");
		const std::string& name = $2;
		FunctionProtoType* fp = module.getProtoType(name, dataTypeList);
		if(fp == NULL){
			fp = new FunctionProtoType(name, dataTypeList, $<val>1); //find the prototype in the module. if not found, add a new one
			module.addProtoType(*fp);
		}
		dataTypeList.clear();
		Function& f = *new Function(name, *fp);
		IcErr err = module.addFunction(f);
		if(err != eNoErr)
			yyerror(errMsg[err]);
	} ;

statement_block: statement_block statement |  ;
	
statement: declaration 
	| assignment
	| func_call
	| return_stmt
	| ';' {trace2("statement ");}
	;
declaration: datatype varList ';'	{ trace2("declaration ");}
varList: IDENTIFIER	{ addSymbol($1); }
	| varList',' IDENTIFIER { addSymbol($3); }
	;
datatype: INTEGER 	{ trace1("int "); }
	| FLOAT 	{ trace1("float "); }
	| VOID		{ trace1("void "); }
	;
assignment: IDENTIFIER '=' expression ';'	{ trace1("assignment"); } // need to search a better name for assignment. This is a statement
return_stmt: RETURN expression;
expression: NUMBER { $$ = (Value*)new Constant(); }
	| IDENTIFIER { $$ = new Variable(); }
	| expression '+' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Add); }
	| expression '-' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Sub); }
	| expression '*' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Mul); }
	| expression '/' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Div); }
	| func_call { $$ = new FunctionCall(); }
	| '('expression')' { $$ = $2; }
	;
func_call: IDENTIFIER'('paramlist')' { trace2("function called"); }
paramlist: expression
	| paramlist',' expression
	|
	;
%%

void yyerror(string s) {
    fprintf(stderr, "%s\n", s.c_str());
}

int yywrap (void ) {
	return 1;
}

//Helper functions

static void addSymbol(char *s){ // this should have more info like datatype, scope etc
	IcErr err = module.addSymbol(*new Symbol(*new std::string(s)));
	if(err)
		yyerror(errMsg[err]);
}


static void trace(string s, int level){
#if DEBUG
	if(level >= debugLevel)
		printf("%s",s.c_str());
#endif
}

int yylex(void){
	return lexer.yylex();
}

Module* ParseFile(char *filename){
	ifstream fp;
	fp.open(filename, ios::in);
	lexer.yyrestart(&fp);
	yyparse();
	return &module;	
}
