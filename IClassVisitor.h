#ifndef ICLASSVISITOR_H
#define ICLASSVISITOR_H

//class forward declarations
class Value;
class Expression;
class Variable;
class BinopExpression;
class FunctionCall;
class Statement;
class Assignment;
class ReturnStatement;
class FunctionProtoType;
class Function;
class SymbolTable;
class Symbol;
class Module;
class ExpressionStatement;

class IClassVisitor{
public:
	virtual void Visit(Value& ) = 0;
	virtual void Visit(Expression&) = 0;
	virtual void Visit(Variable&) = 0;
	virtual void Visit(BinopExpression&) = 0;
	virtual void Visit(FunctionCall&) = 0;
	virtual void Visit(Statement&) = 0;
	virtual void Visit(Assignment&) = 0;
	virtual void Visit(ExpressionStatement&) = 0;
	virtual void Visit(ReturnStatement&) = 0;
	virtual void Visit(FunctionProtoType&) = 0;
	virtual void Visit(Function&) = 0;
	virtual void Visit(SymbolTable&) = 0;	
	virtual void Visit(Symbol& ) = 0;
	virtual void Visit(Module& ) = 0;
};
//don't forget to add the const visitor
#endif //ICLASSVISITOR_H
