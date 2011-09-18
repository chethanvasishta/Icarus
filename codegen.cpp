#include "codegen.h"

//definitions
//--------------SymbolTable---------------

IcErr SymbolTable::add(Symbol& sym){
	//do error handling later
	m_symbols.push_back(&sym);
	return No_Err;
}

//---------------Module----------------
Module::Module(const std::string& name): m_name(name), m_symbolTable(*new SymbolTable()){
}


IcErr Module::addFunction(Function& func){
	//do error handling later
	m_functionList.push_back(&func);
	return No_Err;
}

IcErr Module::addSymbol(Symbol& sym){
	//do error handling later
	return m_symbolTable.add(sym);
}

Module::~Module(){
	delete &m_symbolTable;
}
