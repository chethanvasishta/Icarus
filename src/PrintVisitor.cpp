#include "PrintVisitor.h"
#include "codegen.h"
#include <list>
using namespace std;

void PrintVisitor::Visit(Value& ){

}

void PrintVisitor::Visit(Expression&){

}

void PrintVisitor::Visit(Variable& v){
	cout<<"Variable: "<<v.getSymbol().getName()<<endl;
}

void PrintVisitor::Visit(BinopExpression& b){
	cout<<"Binary Expression: "<<endl;
	b.getLeftValue().accept(*this);
	cout<<b.getOperation()<<endl;
	b.getRightValue().accept(*this);
}

void PrintVisitor::Visit(FunctionCall& f){
	cout<<"Function Call: "<<f.getFunctionProtoType().getName()<<endl;
	std::list<Value*>::const_iterator iter = f.getParamList().begin();
	for(; iter != f.getParamList().end(); ++iter)
		(*iter)->accept(*this);
}

void PrintVisitor::Visit(Statement&){
	cout<<"Statement :"<<endl;
}

void PrintVisitor::Visit(Assignment& a){
	cout<<"Assignment: "<<endl;
	a.getLVal().accept(*this);
	a.getRVal().accept(*this);
}

void PrintVisitor::Visit(ReturnStatement& r){
	cout<<"Return: "<<endl;
	if(r.getReturnValue() != NULL)
		r.getReturnValue()->accept(*this);
}

void PrintVisitor::Visit(FunctionProtoType&){

}

void PrintVisitor::Visit(ExpressionStatement& e){
	cout<<"ExpressionStatement: "<<endl;
	e.getExpression().accept(*this);
}

void PrintVisitor::Visit(Function& f){
	cout<<"Function: "<<f.getName()<<endl;
	std::list<Statement*> statementList = f.getStatements();
	std::list<Statement*>::const_iterator iter = statementList.begin();
	for(; iter != statementList.end(); ++iter){
		(*iter)->accept(*this);
	}
}

void PrintVisitor::Visit(SymbolTable&){

}

void PrintVisitor::Visit(Symbol& ){

}

void PrintVisitor::Visit(Module& m){
	cout<<"Module: "<<m.getName()<<endl;
	std::list<Function*>& funcList = m.getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		(*funcIter)->accept(*this);
	}
}

