#include "codegen.h"

//definitions

IcErr Module::addFunction(Function& func){
	//do error handling later
	m_functionList.push_back(func);
	return No_Err;
}
