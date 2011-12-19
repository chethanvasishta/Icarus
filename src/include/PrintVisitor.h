#ifndef PRINTVISITOR_H
#define PRINTVISITOR_H
#include "IClassVisitor.h"
#include <iostream>

class PrintVisitor : public IClassVisitor{
public:
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

//	PrintVisitor(const std::ostream& pout) : m_pout(pout){}
//	PrintVisitor(): m_pout(std::cout){}
private:
//	we need a stream to print to
//	const std::ostream& m_pout;	
};

#endif //PRINTVISITOR_H
