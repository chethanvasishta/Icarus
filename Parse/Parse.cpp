#include "Parse.h"
#include <iostream>
using namespace std;
enum
{
Token_Int,
Token_Float,
Token_Double,
Token_Char,
Token_Var
};
static int cur_tok;
static union tok_val;
{
 
};

int Parse(char *line){
#if DEBUG
	cout<<"Parsing line : "<<line<<endl;
#endif
	while((i = getTok(line,i)) != -1){
		
	}
	
	return 0;
}
