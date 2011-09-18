#ifndef CODEGEN_H
#define CODEGEN_H

//includes if any
#include <list>
#include <string>
#include "icerr.h"
using namespace std;
//definitions
class Statement{
		
};


class FunctionProtoType{

private:
// we want to have a list of args, with their data types

};


class Function{
public:
	Function(const std::string& name, FunctionProtoType& protoType): m_name(name), m_protoType(protoType){}
	~Function();

	//Getter-Setters

	std::string getName() const { return m_name; }
	std::list<Statement>& getStatements() { return m_statementList; }
	
private:
	std::list<Statement> m_statementList;
	std::string m_name;
	FunctionProtoType& m_protoType;
		

	//prevent unintended c++ synthesis
	Function();
	
};


class Module{
public:
	Module(const std::string& name): m_name(name){}

	//Getter-Setters

	std::string getName() const { return m_name; }
	std::list<Function>& getFunctions() { return m_functionList; }

	IcErr addFunction(Function& f);
	
private:
	std::string m_name;
	std::list<Function> m_functionList;
	Module();
};


#endif //codegen.h
