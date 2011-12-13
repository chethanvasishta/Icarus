#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "codegen.h"

class ASTBuilder{ //This will be a builder class only and won't be necessary for further optimizations in the code
public:
	ASTBuilder();

	IcErr addSymbol(Symbol& s);
	Symbol* addSymbol(char *name);
	Symbol* getSymbol(std::string name); //temporary. Need more than a name, like scope etc.
	
	IcErr addFunction(Function& f);
	IcErr addFunction(FunctionProtoType& fp);
	
	IcErr insertStatement(Statement& s);
	Statement* getCurrentStatement();
	IcErr addBranch(Expression& s);
	
	IcErr addProtoType(FunctionProtoType& fp);
	IcErr addProtoType(const std::string& name, int returnType, FunctionProtoType **fp);
	FunctionProtoType* getProtoType(const std::string name, std::list<int> dataTypes);
	FunctionProtoType* getProtoType(const std::string name) { return getProtoType(name, m_dataTypeList); }
	
	
	Function* getFunction(const std::string name);
	bool endCodeBlock() { if(!m_curFunction->endCodeBlock()) m_curFunction = NULL; }

	Module& getModule() { return m_module; }

	//some helpers
	void pushDataType(int type) { m_dataTypeList.push_back(type); }
	void pushArgName(Symbol *arg) { m_argNameList.push_back(arg); }
	void pushError(std::string error) { m_errorList.push_back(error); }
	bool hasErrors() { return m_errorList.size() != 0; }
private:
	Function* m_curFunction;//can be null
	Module& m_module;

	//some helpers during building of AST
	std::list<int> m_dataTypeList; //to store the data types of the arguments while constructing the argList
	std::list<Symbol*> m_argNameList;	
	std::list<std::string> m_errorList;
};

#endif //ASTBUILDER_H
