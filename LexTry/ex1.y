%{
#include <stdio.h>
int yylex(void);
void yyerror(char *);
%}

%token TOKEN_INT NUMBER START VARIABLE

%%
statement: expr '+' expr | expr '-' expr | expr;
expr: NUMBER '*' NUMBER | NUMBER '/' NUMBER | NUMBER;
%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

