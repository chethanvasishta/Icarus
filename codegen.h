#ifndef CODEGEN_H
#define CODEGEN_H

//includes if any
#include <list>
#include <string>
#include "icerr.h"
#include "IClassVisitor.h"
using namespace std;
//definitions
/*
Everything should be a Value
a constant, an identifier, an expression, a function call etc , just like LLVM
*/
class Value{
public:
	virtual void accept(IClassVisitor &){}
};

class Expression: public Value {
public:
	virtual void accept(IClassVisitor &){}
};

/*
Constant should have a datatype and a value
*/
class Constant: public Expression {
public:
	virtual void accept(IClassVisitor &){}
private:
};

class Variable: public Expression {
public:
	Variable(Symbol& s):m_symbol(s){}
	virtual void accept(IClassVisitor &);
private:
	Symbol& m_symbol;//check this
};

class BinopExpression : public Expression {
public:
	enum BinaryOperation{ 
		Add,
		Sub,
		Mul,
		Div		
	};	
	BinopExpression(Value& left,Value& right, BinaryOperation op): m_left(left), m_right(right), m_op(op){}
	virtual void accept(IClassVisitor &);
private:
	Value& m_left;
	Value& m_right;
	BinaryOperation m_op;
	BinopExpression();
};

class FunctionCall : public Expression {
public:
	//Visitors
	virtual void accept(IClassVisitor &){}
private:

};

class Statement : public Value{
public:
	//Visitors
	virtual void accept(IClassVisitor &){}
		
};

class Assignment : public Statement{
public:
	Assignment(Variable& lVal, Value& rVal):m_lval(lVal), m_rval(rVal){}
	//Getter-Setters
	Variable& getLVal() const { return m_lval; }
	Value& getRVal() const { return m_rval; }

	//Visitors
	virtual void accept(IClassVisitor &);
private:
	Variable& m_lval;
	Value& m_rval;
	Assignment();
};

class ReturnStatement : public Statement {
public:
	ReturnStatement(Value *value): m_value(value){}

	//Getter-Setters
	Value* getReturnVal() { return m_value; }
	
	//Visitors	
	virtual void accept(IClassVisitor &);
private:
	Value *m_value; //return statement can have NULL expression
};

/*
A factory to provide us objects that derive from value
*/
class ValueFactory{
public:
	Value* createFunctionCall();
	Value* createBinopExpression();
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

	//Visitors
	virtual void accept(IClassVisitor &);	
	
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
	std::list<Statement*>& getStatements() { return m_statementList; }
	FunctionProtoType& getProtoType() const { return m_protoType; }

	IcErr addStatement(Statement& s);

	//overloaded operators
	friend std::ostream& operator<<(std::ostream& stream, const Function& f);

	//Visitors
	virtual void accept(IClassVisitor &);

	
private:
	std::list<Statement*> m_statementList;
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

	//Visitors
	virtual void accept(IClassVisitor &);
private:
	Symbol();
	std::string m_name; //we need to add more details regarding the 
};

class SymbolTable{
public:
	//Getter-Setters
	IcErr add(Symbol& sym);	
	std::list<Symbol*>& getSymbols() { return m_symbols; }	

	//Visitors
	virtual void accept(IClassVisitor &);

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
	IcErr insertStatement(Function& f, Statement& s);

	//overloaded operators
	friend std::ostream& operator<<(std::ostream& stream, const Module& m);

	//Visitors
	virtual void accept(IClassVisitor &);
	
private:
	std::string m_name;
	std::list<Function*> m_functionList;
	std::list<FunctionProtoType*> m_funcProtoList;
	SymbolTable& m_symbolTable;
	Module();
};

class ASTBuilder{ //This will be a builder class only and won't be necessary for further optimizations in the code
public:
	ASTBuilder();

	IcErr addSymbol(Symbol& s);
	IcErr addFunction(Function& f);
	IcErr insertStatement(Statement& s);
	IcErr addProtoType(FunctionProtoType& fp);

	FunctionProtoType* getProtoType(const std::string name, std::list<int> dataTypes);
	Symbol* getSymbol(std::string name); //temporary. Need more than a name, like scope etc.

	Module& getModule() { return m_module; }
private:
	Function* m_curFunction;//can be null. this might not be valid now. See m_tempStatementList's explanation
	Module& m_module;
	std::list<Statement*> m_tempStatementList; //the parser would reduce the statement block and then add the function to the list. We need to a temp list to store the statements and store them to the function.
};


#endif //codegen.h
