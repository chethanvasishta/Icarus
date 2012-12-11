#include "codegen.h"
#include <iostream>

//----------BranchStatement-------------------------

BranchStatement::BranchStatement(Expression& condition){
	addBranch(condition);
}

IcErr BranchStatement::addStatement(Statement& s){
	return m_currentBranch->addStatement(s);
}

IcErr BranchStatement::addBranch(Expression& condition){
	m_currentBranch = new Branch(condition);
	m_branches.push_back(m_currentBranch);
	return eNoErr;
}

bool BranchStatement::endCodeBlock(){
	return m_currentBranch->endCodeBlock();
}

Statement* BranchStatement::getCurrentStatement(){
	Statement* curStatement = m_currentBranch->getCurrentStatement();
	if(curStatement == NULL)
		return this;
}

//----------Branch----------------------------------
IcErr Branch::addStatement(Statement& s){
	if(m_currentInsertStatement == NULL){
		m_statementList.push_back(&s);
		ControlFlowStatement* ctrlStmt = dynamic_cast<ControlFlowStatement*>(&s);
		if(ctrlStmt != NULL)
			m_currentInsertStatement = ctrlStmt;
	} else
		m_currentInsertStatement->addStatement(s);	
	return eNoErr;
}

Statement* Branch::getCurrentStatement(){
	if(m_currentInsertStatement == NULL)
		return NULL;
	return m_currentInsertStatement->getCurrentStatement();	
}

bool Branch::endCodeBlock(){
	if(m_currentInsertStatement == NULL)
		return false;
	if(!m_currentInsertStatement->endCodeBlock())
		m_currentInsertStatement = NULL;
	return true;
}

//---------------WhileStatement----------------
IcErr WhileStatement::addStatement(Statement& s){
	if(m_currentInsertStatement == NULL){ //we are generating statements in the function
		m_statementList.push_back(&s);
		ControlFlowStatement* ctrlStmt = dynamic_cast<ControlFlowStatement*>(&s);
		if(ctrlStmt != NULL)
			m_currentInsertStatement = ctrlStmt;
	} else
		m_currentInsertStatement->addStatement(s);	
	return eNoErr;
}

//---------------ControlFlowStatement----------------
bool ControlFlowStatement::endCodeBlock(){
	if(m_currentInsertStatement == NULL)
		return false;
	if(!m_currentInsertStatement->endCodeBlock())
		m_currentInsertStatement = NULL;
	return true; //either the inner code block ended or we ended our codeblock just now.
}

Statement* ControlFlowStatement::getCurrentStatement(){
	if(m_currentInsertStatement == NULL)
		return this;
	else
		return m_currentInsertStatement->getCurrentStatement();	
}

//--------------FunctionProtoType---------------
bool FunctionProtoType::operator==(const FunctionProtoType& fpOther) const{
	if(m_name != fpOther.m_name)
		return false;
	if(m_argTypeList.size() != fpOther.m_argTypeList.size())
		return false;
	unsigned int n = m_argTypeList.size();
	std::list<Type*>::const_iterator typeIter = m_argTypeList.begin();
	std::list<Type*>::const_iterator otherTypeIter = fpOther.m_argTypeList.begin();
	for(unsigned int i = 0 ; i < n ; ++i, ++typeIter, ++otherTypeIter){
		if(*typeIter != *otherTypeIter)
			return false;	
	}
	return true;
}

//--------------Function------------------
IcErr Function::addStatement(Statement& s){
	if(m_currentInsertStatement == NULL){ //we are generating statements in the function
		m_statementList.push_back(&s);
		ControlFlowStatement* ctrlStmt = dynamic_cast<ControlFlowStatement*>(&s);
		if(ctrlStmt != NULL)
			m_currentInsertStatement = ctrlStmt;
	} else
		m_currentInsertStatement->addStatement(s);
	
	return eNoErr;
}


Statement* Function::getCurrentStatement(){
	if(m_currentInsertStatement == NULL)
		return NULL;
	return m_currentInsertStatement->getCurrentStatement();
}

bool Function::endCodeBlock(){
	if(m_currentInsertStatement == NULL)
		return false;
	if(!m_currentInsertStatement->endCodeBlock())
		m_currentInsertStatement = NULL;
	return true; //either the inner code block ended or we ended our codeblock just now.
}

std::ostream& operator<<(std::ostream& stream, const Function& f){
	stream<<"Function: "<<f.getName()<<endl;
	//print the statements here	
}

IcErr Function::addSymbol(Symbol& sym){
	m_symbolTable.add(sym);
}

Function::~Function(){
    std::list<Statement*>::iterator stIter = m_statementList.begin();
    for(; stIter != m_statementList.end(); ++stIter)
        delete (*stIter);
    m_statementList.clear();
}

//--------------SymbolTable---------------

IcErr SymbolTable::add(Symbol& sym){
	//do error handling later
	m_symbols.push_back(&sym);
	return eNoErr;
}

//---------------Module----------------
Module::Module(const std::string& name): m_name(name), m_symbolTable(*new SymbolTable()){
}


IcErr Module::addFunction(Function& func){
	//do error handling later
	m_functionList.push_back(&func);
	return eNoErr;
}

IcErr Module::addSymbol(Symbol& sym){
	//do error handling later
	return m_symbolTable.add(sym);
}

IcErr Module::addProtoType(FunctionProtoType& fp){
	//do error handling later
	m_funcProtoList.push_back(&fp);
	return eNoErr;
}

FunctionProtoType* Module::getProtoType(const std::string name, std::list<Type*>& dataTypes){
	FunctionProtoType& fp = *new FunctionProtoType(name, dataTypes, *new Type(Type::IntegerTy)); //no need of return type for comparing prototypes
	std::list<FunctionProtoType*>::const_iterator protoIter = m_funcProtoList.begin();
	for(; protoIter != m_funcProtoList.end(); ++protoIter){
		if(**protoIter == fp)
			return *protoIter;
	}
	return NULL; //not found
}

FunctionProtoType* Module::getProtoType(const std::string name){
	std::list<FunctionProtoType*>::const_iterator protoIter = m_funcProtoList.begin();
	for(; protoIter != m_funcProtoList.end(); ++protoIter){
		if((*protoIter)->getName() == name)
			return *protoIter;
	}
	return NULL; //not found
}

IcErr Module::insertStatement(Function& f, Statement& s){
	return f.addStatement(s);
}

std::ostream& operator<<(std::ostream& stream, const Module& m){
	stream<<"Module: "<<m.m_name<<endl;
	return stream;
}

Module::~Module(){
	delete &m_symbolTable;
    std::list<Function*>::iterator funcIter = m_functionList.begin();
    for(; funcIter != m_functionList.end(); ++funcIter){
        delete (*funcIter);
    }
}

Function* Module::getFunction(const std::string name){
	std::list<Function*>::const_iterator iter = m_functionList.begin();
	for(; iter != m_functionList.end(); ++iter){
		if((*iter)->getName() == name)
			return *iter;
	}
	return NULL;
}

//------------------Code generator methods

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

CompEA* BranchStatement::codegen(){
}

CompEA* Function::codegen(){
	cout<<"Creating code for function"<<getName()<<endl;
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
