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
	std::list< ::Statement*>::iterator sIter = f.getStatements().begin();
	for(; sIter != f.getStatements().end(); ++sIter)
		(*sIter)->accept(*this);
}

void GenLLVM::Visit(::SymbolTable&){

}

void GenLLVM::Visit(::Symbol& ){

}

void GenLLVM::Visit(::Module& m){	
	std::list< ::Function* >& funcList = m.getFunctions();

	for(std::list< ::Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){

		 std::vector<const Type*> args;
		 FunctionProtoType& fp = (*funcIter)->getProtoType();
		 std::list<int>::iterator argTypeIter = fp.getTypeList().begin();
		 for(; argTypeIter != fp.getTypeList().end(); ++argTypeIter){
		 	args.push_back(Type::getInt32Ty(getGlobalContext()));
		 }
		 FunctionType *FT = FunctionType::get(Type::getDoubleTy(getGlobalContext()), args, false);
		 Function *F = Function::Create(FT, Function::ExternalLinkage, (*funcIter)->getName(), &m_module);

		 //Set names for the arguments
		 std::list< ::Symbol*>::iterator symIter = (*funcIter)->getArgSymbolList().begin(); //assert that number of symbols equal number of elements in typelist
		 for(Function::arg_iterator argIter = F->arg_begin(); argIter != F->arg_end(); ++argIter, ++symIter)
		 	argIter->setName((*symIter)->getName());
		
		(*funcIter)->accept(*this);
	}
}

void GenLLVM::generateLLVM(::Module &m){
	m.accept(*this);
	m_module.dump();
}

GenLLVM::GenLLVM() : m_module(*new Module("MyModule", getGlobalContext())) {
}

}
