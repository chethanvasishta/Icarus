#include "dotwriter.h"
#include <iostream>
#include <fstream>
#include <list>
using namespace std;

void DotWriter::writeDotFile(char* filename, Module& m){
	m_fileStream.open(filename, ios::out);
	if(m_fileStream.bad()){
		cout<<"Error writing dot file"<<endl;
		return;
	}
	m_fileStream<<" graph {";
	m.accept(*this);
	m_fileStream<<" }";
	m_fileStream.close();
}

void DotWriter::Visit(Value& ){

}

void DotWriter::Visit(Expression&){

}

void DotWriter::Visit(Variable& v){
	m_fileStream<<v.getSymbol().getName()<<endl;
}

void DotWriter::Visit(BinopExpression& b){
	//m_fileStream<<"Binary Expression: "<<endl;
	b.getLeftValue().accept(*this);
	m_fileStream<<b.getOperation()<<endl;
	b.getRightValue().accept(*this);
}

void DotWriter::Visit(FunctionCall& f){
	m_fileStream<<"subgraph {"<<f.getFunction().getName();
	std::list<Value*>::const_iterator iter = f.getParamList().begin();
	for(; iter != f.getParamList().end(); ++iter)
		(*iter)->accept(*this);
	m_fileStream<<"}"<<endl;
}

void DotWriter::Visit(Statement&){
}

void DotWriter::Visit(Assignment& a){
	//m_fileStream<<"Assignment: "<<endl;
	a.getLVal().accept(*this);
	a.getRVal().accept(*this);
}

void DotWriter::Visit(ReturnStatement& r){
	m_fileStream<<"Return: "<<endl;
	if(r.getReturnValue() != NULL)
		r.getReturnValue()->accept(*this);
}

void DotWriter::Visit(FunctionProtoType&){

}

void DotWriter::Visit(ExpressionStatement& e){
	m_fileStream<<"ExpressionStatement: "<<endl;
	e.getExpression().accept(*this);
}

void DotWriter::Visit(Function& f){
	m_fileStream<<"subgraph {"<<f.getName();
	std::list<Statement*> statementList = f.getStatements();
	std::list<Statement*>::const_iterator iter = statementList.begin();
	for(; iter != statementList.end(); ++iter){
		(*iter)->accept(*this);
	}
	m_fileStream<<"}";
}

void DotWriter::Visit(SymbolTable&){

}

void DotWriter::Visit(Symbol& ){

}

void DotWriter::Visit(Module& m){
	m_fileStream<<"Module: "<<m.getName()<<endl;
	std::list<Function*>& funcList = m.getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		(*funcIter)->accept(*this);
	}
}

