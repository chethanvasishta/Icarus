#include <iostream>
#include <cstdio>
#include "codegen.h"
#include "PrintVisitor.h"
#include "./Dot/dotwriter.h"
#include "ILBuilder.h"
#include <cstdlib>
#include "genIL.h"
#include "genllvm.h"
#include <llvm/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <fstream>
extern Module* ParseFile(char *filename); //using this for now. need to create a standard header file for lex

#ifndef DEBUG
#define DEBUG 1
#endif
using namespace std;

//global variables
Module *module;

void genExecutable(){
	int i = system("llvm-as temp.ll"); //check if llvm is installed
	if(!i) i = system("llc temp.bc");
	if(!i) i = system("g++ temp.s");//we will generate a a.out	
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
	bool llvmenabled = true;
	if(llvmenabled){
		GenLLVM genLLVM;
		genLLVM.generateLLVM(*module);
		llvm::Module& llvmModule = genLLVM.getModule();
		llvmModule.dump();

		std::string moduleStr;
		llvm::raw_string_ostream string(moduleStr);
		fstream moduleDumpFile;
		moduleDumpFile.open("temp.ll", fstream::in | fstream::out | fstream::trunc);
		if(moduleDumpFile.is_open()){
			llvmModule.print(string, NULL);
			moduleDumpFile<<moduleStr;
			moduleDumpFile.close();
		}
		genExecutable();

	}else{
		ILBuilder myILBuilder;
		myILBuilder.buildIL(*module); //we have our temp.ll file
		genExecutable();
	}
	return 0;
}

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout<<"should input a file to compile"<<endl;
		return 0;
	}
	for(int i = 1 ; i < argc ; ++i)
		Compile(argv[i]);	
	return 0;
}
