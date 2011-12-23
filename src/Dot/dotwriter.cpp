#include "dotwriter.h"
#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
using namespace std;

void DotWriter::writeDotFile(std::string& filename, Module& m){
	m_fileStream.open(filename.c_str(), ios::out);
	if(m_fileStream.bad()){
		cout<<"Error writing dot file"<<endl;
		return;
	}
	m_fileStream<<" digraph {"<<endl;
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
	m_fileStream<<"subgraph {"<<f.getFunctionProtoType().getName();
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
	std::list<Statement*> statementList = f.getStatements();
	std::list<Statement*>::const_iterator iter = statementList.begin();
	for(; iter != statementList.end(); ++iter){
		(*iter)->accept(*this);
	}
}

void DotWriter::Visit(SymbolTable&){

}

void DotWriter::Visit(Symbol& ){

}

void DotWriter::Visit(Module& m){
	std::string moduleID = getNextName();
	m_fileStream<<moduleID<<"[label=\"Module: "<<m.getName()<<"\"]"<<endl;
	std::list<Function*>& funcList = m.getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		std::string funcID = getNextName();
		m_fileStream<<funcID<<"[label=\"Function: "<<(*funcIter)->getName()<<"\"]"<<endl;
		m_fileStream<<moduleID<<"->"<<funcID<<endl;
	}
}

std::string& DotWriter::getNextName(){
	std::ostringstream os;
	os << "n"<<m_nameseed++;
	return *new std::string(os.str());
}
