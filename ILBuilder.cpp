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

void ILBuilder::buildIL(Module &m){
	m.codegen();
}
