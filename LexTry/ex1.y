%{
#include <stdio.h>
extern "C" {
	int yylex(void);
	void yyerror(char *);
}
extern FILE * yyin;
#define DEBUG 1
%}
%left '+' '-'
%left  '*' '/'


%token INTEGER NUMBER VARIABLE FLOAT

%%
program: program statement
	|
	;
statement: expr
	|
	;
expr:	expr '+' expr 	{ $$ = $1 + $3 ; printf("%d\n", $$); }
	| expr '-' expr { $$ = $1 - $3 ; printf("%d\n", $$); }
	| expr '*' expr { $$ = $1 * $3 ; printf("%d\n", $$); }
	| expr '/' expr { $$ = $1 / $3 ; printf("%d\n", $$); }
	| '(' expr ')' 	{ $$ = $2 ; 	 printf("%d\n", $$); }
	| NUMBER	{ $$ = $1 ; }
	| 
	;

%%
extern "C" {
	void yyerror(char *s) {
	    fprintf(stderr, "%s\n", s);
	}
}

int main(int argc, char *argv[]){
	if(argc >= 2){
		FILE *fp = fopen(argv[1], "r");
		if(fp == NULL) return 1;
		yyin = fp;
#if DEBUG
		printf("setting yyin to input file\n");
#endif
	}
	yyparse();
	return 0;
}

