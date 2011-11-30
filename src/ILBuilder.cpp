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
	switch(b.getOperation()){
		case BinopExpression::Add:
			m_asmOutputFile<<"add "<<"i32 ";			
		 	break;
		 case BinopExpression::Sub:
			m_asmOutputFile<<"sub "<<"i32 ";
		 	break;
		 case BinopExpression::Mul:
			m_asmOutputFile<<"mul "<<"i32 ";
		 	break;
		case BinopExpression::Div:
			m_asmOutputFile<<"div "<<"i32 ";
		 	break;
	}
	b.getLeftValue().accept(*this);
	m_asmOutputFile<<",";
	b.getRightValue().accept(*this);	
}

void ILBuilder::Visit(FunctionCall& f){
	Function& func = f.getFunction();
	m_asmOutputFile<<"call "<<"i32 "<<"@"<<func.getName()<<"(";
	std::list<Value*>::iterator paramIter = f.getParamList().begin();
	int n = f.getParamList().size();
	for(; paramIter != f.getParamList().end(); ++paramIter, --n){
		m_asmOutputFile<<"i32 ";
		(*paramIter)->accept(*this);
		if(n > 1)
			m_asmOutputFile<<",";
	}
	m_asmOutputFile<<")";
}

void ILBuilder::Visit(Statement&){
}

void ILBuilder::Visit(Assignment& a){
	a.getLVal().accept(*this);
	m_asmOutputFile<<" = ";
	a.getRVal().accept(*this);
	m_asmOutputFile<<";"<<endl;
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
