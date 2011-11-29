#include <iostream>
#include <cstdio>
#include "codegen.h"
#include "PrintVisitor.h"
#include "./Dot/dotwriter.h"
#include "ILBuilder.h"
#include <cstdlib>
#include "genIL.h"
extern Module* ParseFile(char *filename); //using this for now. need to create a standard header file for lex

#ifndef DEBUG
#define DEBUG 1
#endif
using namespace std;

//global variables
Module *module;

void genExecutable(){
	int i = system("llvm-as temp.ll"); //check if llvm is installed
	i = system("llc temp.bc");
	i = system("g++ temp.s");//we will generate a a.out	
}

int Compile(char *fileName){
#if DEBUG
	std::cout<<"Compiling "<<fileName<<endl; 
#endif	
	module = ParseFile(fileName); //this function should return us the link to the module created by the parser
	GenIL *myILGen = new GenIL(*module);
	module = myILGen->generateIL();
#if DEBUG
	std::cout<<"Printing the IL"<<endl;
	PrintVisitor p;
	p.Visit(*module);
#endif
	ILBuilder myILBuilder;
	myILBuilder.buildIL(*module); //we have our temp.ll file
	genExecutable();	
	return 0;
}

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout<<"should input a file to compile"<<endl;
		return 0;
	}
	PrintVisitor *p = new PrintVisitor();
	DotWriter *d = new DotWriter();
	for(int i = 1 ; i < argc ; ++i){
		Compile(argv[i]);
		p->Visit(*module);
		//d->writeDotFile("1.dot", *module);
	}
	return 0;
}
