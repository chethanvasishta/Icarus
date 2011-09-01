%{
#include "y.tab.h"
#include <stdio.h>

int last = START;
%}
%% 
"int" { 
	if(last != START) { 
		yyerror("%s is a keyword", yytext); 
	} else {
		last = TOKEN_INT;
		return TOKEN_INT;
	}
	}
[0-9]+ {
	if(last == TOKEN_INT){
		yyerror("%s is not a valid variable\n", yytext);
		last = START;
	}else{
		ECHO;
		return NUMBER;
	}
	}

[a-zA-Z][a-zA-Z0-9]* {
	if(last == TOKEN_INT){
		printf("declaring variable %s\n", yytext);
		last = START;
		return VARIABLE;
	}else{
		ECHO;
		return VARIABLE;
	}
	}

%%

