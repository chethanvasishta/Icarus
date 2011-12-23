#include "ASTBuilder.h"


ASTBuilder::ASTBuilder():m_module(*new Module("globalModule")),m_curFunction(NULL){
}

IcErr ASTBuilder::addSymbol(Symbol& s){
	if(m_curFunction != NULL) //we are in global space
		return m_curFunction->addSymbol(s);
	return m_module.addSymbol(s);
}

IcErr ASTBuilder::addFunction(Function& f){
	IcErr err = m_module.addFunction(f);	
	if(!err)
		m_curFunction = &f;
	return err;
}

IcErr ASTBuilder::addFunction(FunctionProtoType& fp){
	Function& f = *new Function(fp, m_argNameList);
	IcErr err = m_module.addFunction(f);
	m_argNameList.clear();
	if(!err)
		m_curFunction = &f;
	return err;
}

IcErr ASTBuilder::insertStatement(Statement& s){
	return m_curFunction->addStatement(s);
}

IcErr ASTBuilder::addProtoType(FunctionProtoType& fp){
	return m_module.addProtoType(fp);
}

IcErr ASTBuilder::addProtoType(const std::string& name, Type returnType, FunctionProtoType **fpPtr){
	FunctionProtoType& fp = *new FunctionProtoType(name, m_dataTypeList, returnType);
	IcErr err = m_module.addProtoType(fp);
	m_dataTypeList.clear();
	if(fpPtr != NULL)
		*fpPtr = &fp;
	return err;
}

FunctionProtoType* ASTBuilder::getProtoType(const std::string name, std::list<Type*> dataTypes){
	return m_module.getProtoType(name, dataTypes);
}

Symbol* ASTBuilder::getSymbol(std::string name){
	if(m_curFunction != NULL){}
		std::list<Symbol*>& funcSymbolList = m_curFunction->getSymbols();
		std::list<Symbol*>::const_iterator symIter = funcSymbolList.begin();
		for(; symIter!= funcSymbolList.end() ; ++symIter ){
			if((*symIter)->getName() == name)
				return *symIter;
	}		

	//search in the global symbol table
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

FunctionProtoType* ASTBuilder::getFunctionProtoType(const std::string& name){
	return m_module.getProtoType(name);
}

Symbol* ASTBuilder::addSymbol(char *s, Type& type){ // this should have more info like datatype, scope etc
	Symbol *ourSymbol = new Symbol(*new std::string(s), type);
	IcErr err = addSymbol(*ourSymbol);
//	if(err)
//		yyerror(errMsg[err]);
	return ourSymbol;
}

IcErr ASTBuilder::addBranch(Expression& e){
	Statement* stmt = getCurrentStatement();
	if(stmt != NULL){
		BranchStatement *branchStmt = dynamic_cast<BranchStatement*>(stmt);
		if(branchStmt == NULL)
			throw;
		branchStmt->addBranch(e);
	}
}

Statement* ASTBuilder::getCurrentStatement(){
	return m_curFunction->getCurrentStatement();
}

bool ASTBuilder::endCodeBlock(){
	if(!m_curFunction->endCodeBlock())
		m_curFunction = NULL;
}
