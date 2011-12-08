#ifndef CODEGEN_H
#define CODEGEN_H

//includes if any
#include <list>
#include <string>
#include "icerr.h"
#include "IClassVisitor.h"
#include "CompEA.h"

//C++ Macros to use non C++ standard macro usage. See DataTypes.h for more details
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#include <llvm/Value.h>
#include <llvm/Module.h>
using namespace std;
//definitions
/*
Everything should be a Value
a constant, an identifier, an expression, a function call etc , just like LLVM
*/

//forward declarations

class GenIL;
class GenLLVM;

//definitions
class Value{
public:
	virtual void accept(IClassVisitor &){}
	virtual CompEA* codegen() = 0;
	virtual Value* genIL(GenIL*) = 0;
	virtual llvm::Value* genLLVM(GenLLVM*) = 0;
};

class Expression: public Value {
public:
	virtual void accept(IClassVisitor &)=0;
	virtual CompEA* codegen() = 0;
	virtual Value* genIL(GenIL*) = 0;
	virtual llvm::Value* genLLVM(GenLLVM*) = 0;
};

/*
Constant should have a datatype and a value
*/
class Constant: public Expression {
public:

	Constant(int value):m_value(value){}
	
	virtual void accept(IClassVisitor &){}
	virtual CompEA* codegen();

	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);
	
	int getValue(){ return m_value; }
	
private:
	int m_value; //int for now. We need to change it 
	Constant();
};

class Variable: public Expression {
public:
	Variable(Symbol& s):m_symbol(s){}

	//Getter-Setters
	Symbol& getSymbol() { return m_symbol; }
	
	virtual void accept(IClassVisitor &);
	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);
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

	//Getter-Setters	
	Value& getLeftValue() { return m_left; }
	Value& getRightValue() { return m_right; }
	BinaryOperation getOperation() { return m_op; }

	//Visitors
	virtual void accept(IClassVisitor &);
	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);
private:
	Value& m_left;
	Value& m_right;
	BinaryOperation m_op;
	BinopExpression();
};

class FunctionCall : public Expression {
public:
	FunctionCall(Function& function, std::list<Value*>& params): m_function(function), m_paramList(params){}

	//Getter-Setters
	Function& getFunction() { return m_function; }
	std::list<Value*>& getParamList() { return m_paramList; }

	//Visitors
	virtual void accept(IClassVisitor &);
	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);
private:
	Function& m_function;
	std::list<Value*> m_paramList;
};

class Statement : public Value{
public:
	//Visitors
	virtual void accept(IClassVisitor &)=0;
	///!!! make this class abstract
private:	
};

class Assignment : public Statement{
public:
	Assignment(Variable& lVal, Value& rVal):m_lval(lVal), m_rval(rVal){}
	//Getter-Setters
	Variable& getLVal() const { return m_lval; }
	Value& getRVal() const { return m_rval; }

	//Visitors
	virtual void accept(IClassVisitor &);
	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);
private:
	Variable& m_lval;
	Value& m_rval;
	Assignment();
};

class ReturnStatement : public Statement {
public:
	ReturnStatement(Value *value): m_value(value){}

	//Getter-Setters
	Value* getReturnValue() { return m_value; }
	
	//Visitors	
	virtual void accept(IClassVisitor &);
	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);
private:
	Value *m_value; //return statement can have NULL expression
	ReturnStatement();
};

class ExpressionStatement : public Statement {
public:
	ExpressionStatement(Expression& expression): m_expression(expression){}

	Expression& getExpression() { return m_expression; }

	//Visitors	
	virtual void accept(IClassVisitor &);
	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);
private:
	Expression& m_expression;
	ExpressionStatement();

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


class Function{
public:
	Function(FunctionProtoType& protoType, std::list<Symbol*>& argSymbolList): m_protoType(protoType), m_argSymbolList(argSymbolList), m_symbolTable(*new SymbolTable()){}
	~Function();

	//Getter-Setters

	std::string getName() const { return m_protoType.getName(); }
	std::list<Statement*>& getStatements() { return m_statementList; }
	FunctionProtoType& getProtoType() const { return m_protoType; }
	std::list<Symbol*>& getArgSymbolList() { return m_argSymbolList; }
	std::list<Symbol*>& getSymbols() { return m_symbolTable.getSymbols(); }
	SymbolTable& getSymbolTable() { return m_symbolTable; }

	IcErr addStatement(Statement& s);
	IcErr addSymbol(Symbol& sym);

	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);

	//overloaded operators
	friend std::ostream& operator<<(std::ostream& stream, const Function& f);

	//Visitors
	virtual void accept(IClassVisitor &);	
private:
	std::list<Statement*> m_statementList;
	FunctionProtoType& m_protoType;
	std::list<Symbol*> m_argSymbolList;
	SymbolTable& m_symbolTable; //we should have a table for the function locals. this will get precendence over the 

	//prevent unintended c++ synthesis
	Function();
	
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
	Function* getFunction(const std::string name); //need to add support for datatypes too

	IcErr addFunction(Function& f);
	IcErr addSymbol(Symbol& s);
	IcErr addProtoType(FunctionProtoType& fp);
	IcErr insertStatement(Function& f, Statement& s);

	virtual CompEA* codegen();
	virtual Value* genIL(GenIL*);
	virtual llvm::Value* genLLVM(GenLLVM*);

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

#endif //codegen.h
