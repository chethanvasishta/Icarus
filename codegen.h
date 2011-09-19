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
public:
	FunctionProtoType(const std::string& name, std::list<int>& typeList, int returnType): m_name(name), m_argTypeList(typeList), m_returnType(returnType){}

	//Getter-Setters
	std::string getName() const { return m_name; }
	std::list<int>& getTypeList() { return m_argTypeList; }
	int getReturnType() { return m_returnType; }

	//overloaded operators
	bool operator==(const FunctionProtoType& fpOther) const;
	
private:
	std::string m_name;
	std::list<int> m_argTypeList; //just store the datatypes; no need of names for the proto
	int m_returnType;
	FunctionProtoType();
};


class Function{
public:
	Function(const std::string& name, FunctionProtoType& protoType): m_name(name), m_protoType(protoType){}
	~Function();

	//Getter-Setters

	std::string getName() const { return m_name; }
	std::list<Statement>& getStatements() { return m_statementList; }
	FunctionProtoType& getProtoType() const { return m_protoType; }
	
private:
	std::list<Statement> m_statementList;
	std::string m_name;
	FunctionProtoType& m_protoType;

	//prevent unintended c++ synthesis
	Function();
	
};

class Symbol{
public:
	Symbol(std::string& name): m_name(name){}
	~Symbol();

	//Getter-Setters
	std::string getName() const { return m_name; }
private:
	Symbol();
	std::string m_name; //we need to add more details regarding the 
};

class SymbolTable{
public:
	//Getter-Setters
	IcErr add(Symbol& sym);	
	std::list<Symbol*>& getSymbols() { return m_symbols; }	
private:
	std::list<Symbol*> m_symbols;
};


class Module{
public:
	Module(const std::string& name);
	~Module();
	//Getter-Setters

	std::string getName() const { return m_name; }
	std::list<Function*>& getFunctions() { return m_functionList; }
	std::list<Symbol*>& getSymbols() { return m_symbolTable.getSymbols(); }
	FunctionProtoType* getProtoType(const std::string name, std::list<int> dataTypes);

	IcErr addFunction(Function& f);
	IcErr addSymbol(Symbol& s);
	IcErr addProtoType(FunctionProtoType& fp);
	
private:
	std::string m_name;
	std::list<Function*> m_functionList;
	std::list<FunctionProtoType*> m_funcProtoList;
	SymbolTable& m_symbolTable;
	Module();
};


#endif //codegen.h
