%{
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "codegen.h"
#include <FlexLexer.h>
#include <fstream>
#include "ASTBuilder.h"
using namespace std;

int yylex(void);
extern void yyerror(string);
extern "C" FILE *yyin;

//debug definitions
static void trace(string,int);
#define DEBUG 1
#if DEBUG
static int debugLevel = 1;
#endif 

#define trace1(s) trace(s, 1)
#define trace2(s) trace(s, 2)

static ASTBuilder& builder = *new ASTBuilder();
std::list<Value*> parameterList;
yyFlexLexer lexer; //this is our lexer
%}

%union
{
    char*	string;
    int 	integer;
    Value*	value;
    Statement* 	statement;
}


%left '+' '-'
%left  '*' '/'
%nonassoc '(' ')'

%token<integer> INTEGER NUMBER FLOAT VOID RETURN IF ELSE
%token<string> IDENTIFIER

%type<integer> datatype
%type<value> expression func_call
%type<statement> return_stmt assignment

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
		const std::string& string = $2;
		IcErr err = builder.addProtoType(string, $1, NULL);
		if(err != eNoErr)
			yyerror(errMsg[err]);		
	}
	;
	
arglist: datatype IDENTIFIER 
	{
		builder.pushDataType($1);
		Symbol *sym = builder.addSymbol($2);
		builder.pushArgName(sym);
	}
	| arglist ',' datatype IDENTIFIER 
	{
		builder.pushDataType($3);
		Symbol *sym = builder.addSymbol($4);
		builder.pushArgName(sym);
	}
	|
	;
//as and when we find the identifiers , we need to add them to a list and use them while constructing the prototype/func

func_defn: datatype IDENTIFIER '(' arglist ')' '{' 
	{
		trace2("function definition ");
		const std::string& string = $2;
		FunctionProtoType* fp = builder.getProtoType(string);//use current dataTypeList
		if(fp == NULL) //find the prototype in the module. if not found, add a new one
			builder.addProtoType(string, $<integer>1, &fp);
		IcErr err = builder.addFunction(*fp);
		if(err != eNoErr)
			yyerror(errMsg[err]);
	}

	statement_block '}' 
	{	//we should clear the m_curFunction after this, so that any global decl will not be a part of prev function's symtab
		builder.clearCurrentFunctionPtr();	
	}
	;
	
statement_block: statement_block statement |  ;
	
statement: declaration 
	| assignment { builder.insertStatement(*$1);}
	| expression';' { builder.insertStatement(*new ExpressionStatement(*(Expression *)$1));}
	| return_stmt ';'{ builder.insertStatement(*$1);}
	| if_else_stmt
	| ';' {trace2("statement ");}
	;

if_else_stmt: IF '(' expression ')' ifblock;

ifblock: '{' statement_block '}' | statement;
	
declaration: datatype varList ';'	{ trace2("declaration ");}

varList: IDENTIFIER	{ builder.addSymbol($1); }
	| varList',' IDENTIFIER { builder.addSymbol($3); }
	;
	
datatype: INTEGER 	{ trace1("int "); }
	| FLOAT 	{ trace1("float "); }
	| VOID		{ trace1("void "); }
	;
	
assignment: IDENTIFIER '=' expression ';'	
	{ 
		trace1("assignment");
		Symbol *identifierSymbol = builder.getSymbol($1);
		if(identifierSymbol == NULL)
			yyerror("Symbol Not Defined");
		$$ = new Assignment(*new Variable(*identifierSymbol), *$3);
	}

return_stmt: RETURN expression { $$ = new ReturnStatement($2);};
	| RETURN { $$ = new ReturnStatement(NULL);};
	; 

expression: NUMBER { $$ = new Constant($1); }
	| IDENTIFIER {
		Symbol *identifierSymbol = builder.getSymbol($1);
		if(identifierSymbol == NULL)			
			yyerror("Symbol Not Defined");			
		$$ = new Variable(*identifierSymbol); 
	}
	| expression '+' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Add); }
	| expression '-' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Sub); }
	| expression '*' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Mul); }
	| expression '/' expression { $$ = new BinopExpression(*$1, *$3, BinopExpression::Div); }
	| func_call { $$ = $1; }
	| '('expression')' { $$ = $2; }
	;
	
func_call: IDENTIFIER'('paramlist')'
	{
		trace2("function called");
		Function* func = builder.getFunction($1);
		if(func == NULL)
			yyerror("Function not found");
		$$ = new FunctionCall(*func, parameterList);
		parameterList.clear();
	}

paramlist: expression { parameterList.push_back($1); }
	| paramlist',' expression { parameterList.push_back($3); }
	|
	;
%%

void yyerror(string s) {
    fprintf(stderr, "%s\n", s.c_str());
    builder.pushError(s);
}

int yywrap (void ) {
	return 1;
}

//Helper functions
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
	if(!fp.is_open()){
		fprintf(stderr, "Oops! Couldn't open file %s\n!", filename);
		return NULL;
	}
	lexer.yyrestart(&fp);
	yyparse();
	if(builder.hasErrors()){
		fprintf(stderr, "Stopping compilation as we found some syntax errors in %s\n!", filename);
		return NULL;
	}
	return &builder.getModule();	
}
