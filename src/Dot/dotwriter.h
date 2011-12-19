#ifndef DOTWRITER_H
#define DOTWRITER_H
#include "codegen.h"
#include "IClassVisitor.h"
#include <fstream>
class DotWriter : IClassVisitor {
public:
	DotWriter(): m_nameseed(0){}
	virtual void Visit(Value& );
	virtual void Visit(Expression&);
	virtual void Visit(Variable&);
	virtual void Visit(BinopExpression&);
	virtual void Visit(FunctionCall&);
	virtual void Visit(Statement&);
	virtual void Visit(ExpressionStatement&);	
	virtual void Visit(Assignment&);
	virtual void Visit(ReturnStatement&);
	virtual void Visit(FunctionProtoType&);
	virtual void Visit(Function&);
	virtual void Visit(SymbolTable&);	
	virtual void Visit(Symbol& );
	virtual void Visit(Module& );
	void writeDotFile(std::string& filename, Module& m);
private:
	ofstream m_fileStream;
	long m_nameseed;
	std::string& getNextName();
};
#endif
