#include "codegen.h"
#include <iostream>

//definitions
//--------------Variable----------------------

void Variable::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

//--------------BinopExpression----------------

void BinopExpression::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

//------------Assignment--------------------

void Assignment::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

//------------ReturnStatement------------------

void ReturnStatement::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

//---------ExpressionStatement-----------------

void ExpressionStatement::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

//-----------FunctionCall----------------------------
void FunctionCall::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}



//--------------FunctionProtoType---------------
bool FunctionProtoType::operator==(const FunctionProtoType& fpOther) const{
	if(m_name != fpOther.m_name)
		return false;
	if(m_argTypeList.size() != fpOther.m_argTypeList.size())
		return false;
	unsigned int n = m_argTypeList.size();
	std::list<int>::const_iterator typeIter = m_argTypeList.begin();
	std::list<int>::const_iterator otherTypeIter = fpOther.m_argTypeList.begin();
	for(unsigned int i = 0 ; i < n ; ++i, ++typeIter, ++otherTypeIter){
		if(*typeIter != *otherTypeIter)
			return false;	
	}
	return true;
}

void FunctionProtoType::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}
//--------------Function------------------
IcErr Function::addStatement(Statement& s){
	m_statementList.push_back(&s);
	return eNoErr;
}

std::ostream& operator<<(std::ostream& stream, const Function& f){
	stream<<"Function: "<<f.m_name<<endl;
	//print the statements here
	
}

void Function::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

//------------Symbol---------------

void Symbol::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

//--------------SymbolTable---------------

IcErr SymbolTable::add(Symbol& sym){
	//do error handling later
	m_symbols.push_back(&sym);
	return eNoErr;
}

void SymbolTable::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
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

FunctionProtoType* Module::getProtoType(const std::string name, std::list<int> dataTypes){
	FunctionProtoType& fp = *new FunctionProtoType(name, dataTypes, 0); //no need of return type for comparing prototypes
	std::list<FunctionProtoType*>::const_iterator protoIter = m_funcProtoList.begin();
	for(; protoIter != m_funcProtoList.end(); ++protoIter){
		if(**protoIter == fp)
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
}

void Module::accept(IClassVisitor &visitor){
	visitor.Visit(*this);
}

Function* Module::getFunction(const std::string name){
	std::list<Function*>::const_iterator iter = m_functionList.begin();
	for(; iter != m_functionList.end(); ++iter){
		if((*iter)->getName() == name)
			return *iter;
	}
	return NULL;
}


//----------------------ASTBuilder----------------
ASTBuilder::ASTBuilder():m_module(*new Module("globalModule")),m_curFunction(NULL){
}

IcErr ASTBuilder::addSymbol(Symbol& s){
	return m_module.addSymbol(s);
}

IcErr ASTBuilder::addFunction(Function& f){
	IcErr err = m_module.addFunction(f);	
	if(!err){
		m_curFunction = &f;
		std::list<Statement*>::const_iterator iter = m_tempStatementList.begin();
		for(; iter != m_tempStatementList.end(); ++iter){
			err = f.addStatement(*(*iter));
			if(err)
				break;
		}
	}
	m_tempStatementList.clear();
	return err;
}

IcErr ASTBuilder::insertStatement(Statement& s){
	m_tempStatementList.push_back(&s);
	return eNoErr;// m_module.insertStatement(*m_curFunction, s);
}

IcErr ASTBuilder::addProtoType(FunctionProtoType& fp){
	return m_module.addProtoType(fp);
}

FunctionProtoType* ASTBuilder::getProtoType(const std::string name, std::list<int> dataTypes){
	return m_module.getProtoType(name, dataTypes);
}

Symbol* ASTBuilder::getSymbol(std::string name){
	std::list<Symbol*>& symbolList = m_module.getSymbols();
	std::list<Symbol*>::const_iterator iter = symbolList.begin();
	for(; iter!= symbolList.end() ; ++iter ){
		if((*iter)->getName() == name)
			return *iter;		
	}
	return NULL;
}

Function* ASTBuilder::getFunction(const std::string name){
	return m_module.getFunction(name);
}
