%{
#include <stdio.h>
extern "C"
{
int yylex(void);
void yyerror(char *);
}
%}

%token TOKEN_INT NUMBER VARIABLE

%%
/*statement: expr '+' expr { $$ = $1 + $3; printf("%d", $$); }
	| expr '-' expr { $$ = $1 - $3; printf("%d", $$); }
	| expr { $$ = $1; printf("%d", $$); }
	;*/
program: program statement '\n'
	|
	;
statement: NUMBER '+' NUMBER { $$ = $1 + $3 ; printf("%d\n", $$); } 
	|
	;
//expr: NUMBER;
%%
extern "C"
{

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}
}

int main(){
	yyparse();
	return 0;
}

