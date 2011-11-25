#ifndef ILBUILDER_H
#define ILBUILDER_H

#include "IClassVisitor.h"
#include "CompEA.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
/*
This class takes in a module, goes through each function and generates three address instructions for each statement
1. need a variable to get the next temp variable. compilers generally use _Mtx, x >= 0, as temp variable names
2. we need a stream to store the IL code stream
3. Each function will have a code stream
*/
//forward declarations
class CodeStream;
class Instruction;

class ILBuilder{
public:
	void buildIL(Module& module);	
private:
	//private functions
	void buildFunctionIL(Function &f);
	
	std::list<CodeStream*> m_codestreams;
	CodeStream* m_curCodeStream;
	ofstream m_asmOutputFile;
};



/*
Each function will be converted to a codestream
*/
class CodeStream {
public:
	CodeStream(const std::string& name):m_name(name){}
private:
	std::list<Instruction*> m_instructions;
	std::string m_name;
	CodeStream();
	//need a field to map label to instructions	
};

/*
This is the base class for all instructions, like mov, add, etc
*/
class Instruction {

};

class ILAdd : public Instruction {
public:

private:
	CompEA m_left;
	CompEA m_right;
};

class ILMov : public Instruction {

};

#endif
