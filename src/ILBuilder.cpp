#include "ILBuilder.h"
#include "codegen.h"
#include <iostream>
using namespace std;

CompEA* Constant::codegen(){

}

CompEA* Variable::codegen(){

}

CompEA* BinopExpression::codegen(){

}

CompEA* FunctionCall::codegen(){

}

CompEA* Assignment::codegen(){
	CompEA *right = m_lval.codegen();	
	CompEA *left = m_rval.codegen();
	cout<<"lea "<<right;
	cout<<"mov "<<"["<<left<<"],"<<right;
	return NULL; //we wont use it anyway
}

CompEA* ReturnStatement::codegen(){

}

CompEA* ExpressionStatement::codegen(){

}

CompEA* Function::codegen(){
	cout<<"Creating code for function"<<m_name<<endl;
	std::list<Statement*> statementList = getStatements();
	std::list<Statement*>::const_iterator iter = statementList.begin();
	for(; iter != statementList.end(); ++iter){
		(*iter)->codegen();
	}
}

CompEA* Module::codegen(){
	std::list<Function*>& funcList = getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		(*funcIter)->codegen();
	}
}

void ILBuilder::buildFunctionIL(Function &f){
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
	std::list<Statement*>::const_iterator iter = statementList.begin();
	for(; iter != statementList.end(); ++iter){
		//generate code for the statements
	}
	m_asmOutputFile<<";generate a dummy return statement"<<endl;
	m_asmOutputFile<<"ret i32 0"<<endl;
	m_asmOutputFile<<"}"<<endl;
}

void ILBuilder::buildIL(Module &m){
	m_asmOutputFile.open("temp.ll");
	if(!m_asmOutputFile.is_open()) //return an err ideally
		return;
	m_asmOutputFile << "; Writing assembly code for "<<m.getName()<<endl;
	std::list<Function*>& funcList = m.getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		buildFunctionIL(**funcIter);
	}
	m_asmOutputFile.close();
}
