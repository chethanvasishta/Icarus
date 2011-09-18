#include <iostream>
#include <cstdio>
#include "codegen.h"
extern Module* ParseFile(char *filename); //using this for now. need to create a standard header file for lex

#ifndef DEBUG
#define DEBUG 1
#endif
using namespace std;

//global variables
Module *module;


int Compile(char *fileName){
#if DEBUG
	std::cout<<"Compiling "<<fileName<<endl; 
#endif	
	module = ParseFile(fileName); //this function should return us the link to the module created by the parser
	return 0;
}

static void printFunctions(Module& module){
#if DEBUG
	std::list<Function*>& funcList = module.getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		printf("%s\n", (*funcIter)->getName().c_str());
	}
#endif
}

static void printSymbols(Module& module){
#if DEBUG
	std::list<Symbol*>& symList = module.getSymbols();
	for(std::list<Symbol*>::const_iterator symIter = symList.begin(); symIter != symList.end() ; ++symIter){
		printf("%s\n", (*symIter)->getName().c_str());
	}
#endif
}

static void printModule(Module& module){
#if DEBUG
	cout<<"---------------------Print Module: "<<module.getName()<<"-------------------"<<endl;
	cout<<"---------------------Functions-------------------"<<endl;
	printFunctions(module);
	cout<<"---------------------Symbols---------------------"<<endl;
	printSymbols(module);
#endif
}

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout<<"should input a file to compile"<<endl;
		return 0;
	}
	Compile(argv[1]);
	printModule(*module);
	return 0;
}
