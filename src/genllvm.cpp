#include "genllvm.h"
#include "codegen.h"
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/LLVMContext.h>

namespace llvm{

void GenLLVM::Visit(::Value& ){

}

void GenLLVM::Visit(::Expression&){

}

void GenLLVM::Visit(::Variable& v){
	
}

void GenLLVM::Visit(::BinopExpression& b){
	
}

void GenLLVM::Visit(::FunctionCall& f){
	
}

void GenLLVM::Visit(::Statement&){
}

void GenLLVM::Visit(::Assignment& a){
	
}

void GenLLVM::Visit(::ReturnStatement& r){
	
}

void GenLLVM::Visit(::FunctionProtoType&){

}

void GenLLVM::Visit(::ExpressionStatement& e){
}

void GenLLVM::Visit(::Function& f){
	
}

void GenLLVM::Visit(::SymbolTable&){

}

void GenLLVM::Visit(::Symbol& ){

}

void GenLLVM::Visit(::Module& m){	
	std::list< ::Function* >& funcList = m.getFunctions();

	for(std::list< ::Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){		
		(*funcIter)->accept(*this);
	}
}

void GenLLVM::generateLLVM(::Module &m){
	m.accept(*this);		
}

}
