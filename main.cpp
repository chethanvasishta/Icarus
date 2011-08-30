#include <iostream>
#include <cstdio>

#include "Parse/Parse.h"
#define DEBUG 1
using namespace std;
//char *keywordslist = {"int", "float", "double", "char"};





int Compile(char *fileName){
#if DEBUG
	std::cout<<"Compiling "<<fileName<<endl; 
#endif
	FILE *file = fopen(fileName, "r");
	if(file == NULL){
		cout<<"Couldn't open file"<<endl;
		return 0;
	}
	
	//we start the parsing phase now. Get the tokens and identify the statements
	char buf[256];
	while(! feof(file)){
		if(fgets(buf, 256, file) != NULL){
			Parse(buf);
		}
	}
	fclose(file);  
	return 0;
}

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout<<"should input a file to compile"<<endl;
		return 0;
	}else{
		Compile(argv[1]);
	}
	return 0;
}
