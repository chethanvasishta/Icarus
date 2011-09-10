%{

#include "y.tab.hh"
#include <stdio.h>

%}

%% 

"int" { return INTEGER; }

"float" { return FLOAT; }

[0-9]+ { yylval.val = atoi(yytext);  return NUMBER; }

[a-zA-Z][a-zA-Z0-9]* { yylval.name = yytext; return VARIABLE;	}

[ \t]+ ;

[-+*/;=,] { return *yytext; }

%%

