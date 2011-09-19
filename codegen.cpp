#include "codegen.h"

//definitions
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

FunctionProtoType* Module::getProtoType(const std::string name, std::list<int> dataTypes){
	FunctionProtoType& fp = *new FunctionProtoType(name, dataTypes, 0); //no need of return type for comparing prototypes
	std::list<FunctionProtoType*>::const_iterator protoIter = m_funcProtoList.begin();
	for(; protoIter != m_funcProtoList.end(); ++protoIter){
		if(**protoIter == fp)
			return *protoIter;
	}
	return NULL; //not found
}

Module::~Module(){
	delete &m_symbolTable;
}
