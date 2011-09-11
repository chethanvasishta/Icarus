%{

#include "y.tab.hh"
#include <stdio.h>

%}

%% 

"int" { return INTEGER; }

"float" { return FLOAT; }

[-+*/;=,)(] { return *yytext; }

[0-9]+ { yylval.val = atoi(yytext);  return NUMBER; }

[a-zA-Z][a-zA-Z0-9]* { 
	//yylval.name = yytext; 
	yylval.name = (char*)calloc(yyleng+1, sizeof(char));
        strncpy(yylval.name, yytext, yyleng); // Remember you need to free this.        
	return VARIABLE; 
	}

[ \t]+ ;
%%

