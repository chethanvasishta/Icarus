#include "ILBuilder.h"
#include "codegen.h"
#include <iostream>
using namespace std;

void ILBuilder::Visit(Value& ){

}

void ILBuilder::Visit(Expression&){

}

void ILBuilder::Visit(Variable& v){
	m_asmOutputFile<<"%"<<v.getSymbol().getName();
}

void ILBuilder::Visit(BinopExpression& b){
	//m_fileStream<<"Binary Expression: "<<endl;
	m_asmOutputFile<<b.getLeftValue().accept(*this);
	char c = '+'; //we can make it a string
	switch(b.getOperation()){
		case Add: c = '+'; break;
		case Sub: c = '-'; break;
		case Mul: c = '*'; break;
		case Div: c = '/'; break;
	}
	m_asmOutputFile<<c;
	m_asmOutputFile<<b.getRightValue().accept(*this);	
}

void ILBuilder::Visit(FunctionCall& f){
	Function& func = f.getFunction();
	m_asmOutputFile<<func.getName()<<"()";
}

void ILBuilder::Visit(Statement&){
}

void ILBuilder::Visit(Assignment& a){
	m_asmOutputFile<<a.getLVal().accept(*this)<<" = ";
	a.getRVal().accept(*this);
}

void ILBuilder::Visit(ReturnStatement& r){
}

void ILBuilder::Visit(FunctionProtoType&){

}

void ILBuilder::Visit(ExpressionStatement& e){
}

void ILBuilder::Visit(Function& f){
	//cout<<"Creating code for function"<<f.getName()<<endl;
	m_asmOutputFile<<"define "<<"i32 " /* for now */<<"@" /*needs to be before function name*/<<f.getName()<<"(";
	//output the params
	FunctionProtoType& proto = f.getProtoType();
	std::list<int>::iterator typeListIter = proto.getTypeList().begin();
	std::list<Symbol*>::iterator argSymbolListIter = f.getArgSymbolList().begin();
	unsigned int numArgs = proto.getTypeList().size();
	for(unsigned int i = 0 ; argSymbolListIter != f.getArgSymbolList().end() ; ++i, ++argSymbolListIter){
		//get the type,
		m_asmOutputFile<<"i32 "<<"%"<<(*argSymbolListIter)->getName();
		if((i+1) != numArgs)
			m_asmOutputFile<<",";
	}
	
	m_asmOutputFile<<")"<<"{"<<endl;
	std::list<Statement*> statementList = f.getStatements();
	std::list<Statement*>::iterator statementPtrIter = statementList.begin();
	for(; statementPtrIter != statementList.end(); ++statementPtrIter){
		//generate code for the statements
		(*statementPtrIter)->accept(*this);
	}
	m_asmOutputFile<<";generate a dummy return statement"<<endl;
	m_asmOutputFile<<"ret i32 0"<<endl;
	m_asmOutputFile<<"}"<<endl;
}

void ILBuilder::Visit(SymbolTable&){

}

void ILBuilder::Visit(Symbol& ){

}

void ILBuilder::Visit(Module& m){	
	std::list<Function*>& funcList = m.getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		(*funcIter)->accept(*this);
	}
}

void ILBuilder::buildIL(Module &m){
	m_asmOutputFile.open("temp.ll");
	if(!m_asmOutputFile.is_open()) //return an err ideally
		return;
	m_asmOutputFile << "; Writing assembly code for "<<m.getName()<<endl;
	m.accept(*this);
	m_asmOutputFile.close();
}
