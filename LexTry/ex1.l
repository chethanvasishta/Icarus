%{
#include "y.tab.hh"
#include <stdio.h>
#define DEBUG 1
%}
%% 
"int" return TOKEN_INT;

[0-9]+ { yylval = atoi(yytext);  return NUMBER; }

[a-zA-Z][a-zA-Z0-9]* { return VARIABLE;	}

[ \t]+ ;

[-+*/] { return *yytext; }



%%

