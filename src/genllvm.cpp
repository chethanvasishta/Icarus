#include "genllvm.h"
#include "codegen.h"
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/LLVMContext.h>
using llvm::Type;
using llvm::BasicBlock;
inline llvm::LLVMContext& getGlobalContext(){ //just a double dispatch
	return llvm::getGlobalContext();
}

llvm::Value* Constant::genLLVM(GenLLVM*){

}

llvm::Value* Variable::genLLVM(GenLLVM* g){
	//const llvm::Function* curFunc = g->getBuilder().GetInsertPoint()->getParent();
	return g->getNamedValues()[getSymbol().getName()];
}

llvm::Value* BinopExpression::genLLVM(GenLLVM* g){
	llvm::IRBuilder<>& builder = g->getBuilder();
	switch(getOperation()){
		case Add:
			return builder.CreateAdd(getLeftValue().genLLVM(g), getRightValue().genLLVM(g), "");
		 	break;
		 case Sub:
 			return builder.CreateSub(getLeftValue().genLLVM(g), getRightValue().genLLVM(g), "");			
		 	break;
		 case Mul:
			return builder.CreateMul(getLeftValue().genLLVM(g), getRightValue().genLLVM(g), "");			
		 	break;
		case Div:
			return builder.CreateSDiv(getLeftValue().genLLVM(g), getRightValue().genLLVM(g), "");
		 	break;
	}
}

llvm::Value* FunctionCall::genLLVM(GenLLVM*){

}

llvm::Value* Assignment::genLLVM(GenLLVM* g){
	return g->getBuilder().CreateStore(getLVal().genLLVM(g), getRVal().genLLVM(g), false);
}

llvm::Value* ReturnStatement::genLLVM(GenLLVM* g){

}

llvm::Value* ::ExpressionStatement::genLLVM(GenLLVM*){

}

llvm::Value* Function::genLLVM(GenLLVM* g){

	std::vector<const Type*> args;
	FunctionProtoType& fp = getProtoType();
	std::list<int>::iterator argTypeIter = fp.getTypeList().begin();
	for(; argTypeIter != fp.getTypeList().end(); ++argTypeIter){
		args.push_back(Type::getInt32Ty(getGlobalContext()));
	}
	
	llvm::FunctionType *FT = llvm::FunctionType::get(Type::getDoubleTy(getGlobalContext()), args, false); //set the proper return type
	llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, getName(), &g->getModule());

	BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
	g->getBuilder().SetInsertPoint(BB);
	
	//Set names for the arguments and allocate them
	 //assert that number of symbols equal number of elements in typelist
	llvm::Function::arg_iterator argIter = F->arg_begin();
	for(std::list<Symbol*>::iterator symIter = getArgSymbolList().begin();
		argIter != F->arg_end(); ++argIter, ++symIter){
		argIter->setName((*symIter)->getName());

		//m_irBuilder.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0, (*symIter)->getName()); //its creating new names, I guess we can use the ones in the function definition itself
		g->getNamedValues()[argIter->getName()] = &(*argIter);
	}

	//assume only the local symbols are visited now
	std::list<Symbol*>::iterator symIter = getSymbols().begin();
	for(; symIter != getSymbols().end(); ++symIter){		
		g->getNamedValues()[(*symIter)->getName()] = g->getBuilder().CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0, (*symIter)->getName());
	}
	
	std::list<Statement*>::iterator sIter = getStatements().begin();
	for(; sIter != getStatements().end(); ++sIter)
		(*sIter)->genLLVM(g);
}

llvm::Value* Module::genLLVM(GenLLVM* g){
	std::list<Function*>& funcList = getFunctions();	
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		(*funcIter)->genLLVM(g);
	}
	return NULL; //we wont use it anyway. This function should actually return nothing
}

void GenLLVM::generateLLVM(Module &m){
	m.genLLVM(this);
	m_module.dump();
}

GenLLVM::GenLLVM() : m_module(*new llvm::Module("MyModule", getGlobalContext())), m_irBuilder(*new llvm::IRBuilder<>(getGlobalContext())) {
}
