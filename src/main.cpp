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
#include <unistd.h>
#include "debug.h"
extern Module* ParseFile(char *filename); //using this for now. need to create a standard header file for lex

static Trace& gTrace = Trace::getInstance();
static Debug& gDebug = Debug::getInstance();

using namespace std;

//global variables
Module *module;

void genExecutable(){
	int i = system("llvm-as temp.ll"); //check if llvm is installed
	if(!i) i = system("llc temp.bc");
	if(!i) i = system("g++ temp.s");//we will generate a a.out	
}

int Compile(char *fileName){

	gTrace<<"Compiling file"<<fileName;
	module = ParseFile(fileName); //this function should return us the link to the module created by the parser
	if(module == NULL)
		return -1; //there was some syntax error. Hence we skip all other checks
	GenIL *myILGen = new GenIL(*module);
	module = myILGen->generateIL();

	gTrace<<"Printing the IL";
	if(gDebug.isDebuggable()){
		PrintVisitor p;
		p.Visit(*module);
	}

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
	int option; //to read command line options
	while ((option = getopt (argc, argv, "v")) != -1){
		switch (option){
			case 'v': Debug::getInstance().setDebug(true);
				break;				
		}
	}
	Trace::getInstance()<<"Verbose on!\n";
	for(int i = optind ; i < argc ; ++i)
		Compile(argv[i]);	
	return 0;
}
