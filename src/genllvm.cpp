#include "genllvm.h"
#include "codegen.h"
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/Constants.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>

using llvm::Type;
using llvm::BasicBlock;
inline llvm::LLVMContext& getGlobalContext(){ //just a double dispatch
	return llvm::getGlobalContext();
}

llvm::Value* Constant::genLLVM(GenLLVM* g){
	return g->getBuilder().getInt32(getValue());
}

llvm::Value* Variable::genLLVM(GenLLVM* g){
	//const llvm::Function* curFunc = g->getBuilder().GetInsertPoint()->getParent();
	return g->getBuilder().CreateLoad(g->getNamedValues()[getSymbol().getName()],"");
}

llvm::Value* BinopExpression::genLLVM(GenLLVM* g){
	llvm::IRBuilder<>& builder = g->getBuilder();
	
	llvm::Value* leftValue = getLeftValue().genLLVM(g);	
	llvm::Value* rightValue = getRightValue().genLLVM(g);
		
	switch(getOperation()){
		case Add:
			return builder.CreateAdd(leftValue, rightValue, "");
		 	break;
		 case Sub:
 			return builder.CreateSub(leftValue, rightValue, "");			
		 	break;
		 case Mul:
			return builder.CreateMul(leftValue, rightValue, "");			
		 	break;
		case Div:
			return builder.CreateSDiv(leftValue, rightValue, "");
		 	break;
	}
}

//Helper function to get a llvm function type from our function
llvm::FunctionType& getFunctionType(Function& f){
	std::vector<Type*> args;
	FunctionProtoType& fp = f.getProtoType();
	std::list<int>::iterator argTypeIter = fp.getTypeList().begin();
	for(; argTypeIter != fp.getTypeList().end(); ++argTypeIter){
		args.push_back(Type::getInt32Ty(getGlobalContext()));
	}
	llvm::FunctionType *FT = llvm::FunctionType::get(Type::getInt32Ty(getGlobalContext()), *new llvm::ArrayRef<Type*>(args), false); //set the proper return type
	return *FT;
}

llvm::Value* FunctionCall::genLLVM(GenLLVM* g){
	std::vector<llvm::Value*> paramArrayRef;
	std::list<Value*> paramList = getParamList();
	std::list<Value*>::iterator paramIter = paramList.begin();	
	for(; paramIter != paramList.end(); ++paramIter){
		//llvm::Value* param = g->getBuilder().CreateLoad((*paramIter)->genLLVM(g),"");
		//paramArrayRef.push_back(param);
		paramArrayRef.push_back((*paramIter)->genLLVM(g));
	}

	llvm::FunctionType *FT = &getFunctionType(getFunction());
	llvm::Function *F = static_cast<llvm::Function*>(g->getModule().getOrInsertFunction(getFunction().getName(), FT));
	
	return g->getBuilder().CreateCall(F, *new llvm::ArrayRef<llvm::Value*>(paramArrayRef), "");
}

llvm::Value* Assignment::genLLVM(GenLLVM* g){
	return g->getBuilder().CreateStore(getRVal().genLLVM(g),g->getNamedValues()[getLVal().getSymbol().getName()], false);
}

llvm::Value* ReturnStatement::genLLVM(GenLLVM* g){
	if(getReturnValue() != NULL){		
		//return g->getBuilder().CreateRet(g->getBuilder().CreateLoad(getReturnValue()->genLLVM(g),""));
		return g->getBuilder().CreateRet(getReturnValue()->genLLVM(g));//,"");
	}
	return g->getBuilder().CreateRetVoid();
}

llvm::Value* ExpressionStatement::genLLVM(GenLLVM* g){
	return getExpression().genLLVM(g);
}

llvm::Value* WhileStatement::genLLVM(GenLLVM *g){

	llvm::IRBuilder<>& builder = g->getBuilder();
		
	BasicBlock* curBlock = builder.GetInsertBlock();
	llvm::Function *func = curBlock->getParent();
	BasicBlock *whileBB = BasicBlock::Create(getGlobalContext(), "whilecond", func);
	BasicBlock *whileBodyBB = BasicBlock::Create(getGlobalContext(), "whileblock", func);
	BasicBlock *postWhileBB = BasicBlock::Create(getGlobalContext(), "postwhile", func);
	builder.CreateBr(whileBB);
	builder.SetInsertPoint(whileBB);

	llvm::Value* condition = getCondition().genLLVM(g);
	llvm::Value* zeroConst = llvm::ConstantInt::get(getGlobalContext(), llvm::APInt(32, llvm::StringRef("0"), 0));
	llvm::Value* compareInst = builder.CreateICmpEQ(condition, zeroConst, "");
	builder.CreateCondBr(compareInst, whileBodyBB, postWhileBB);

	builder.SetInsertPoint(whileBodyBB);
	
	std::list<Statement*>::iterator sIter = getStatements().begin();
	for(; sIter != getStatements().end(); ++sIter)
		(*sIter)->genLLVM(g);

	builder.CreateBr(whileBB); //loop back to while condition

	builder.SetInsertPoint(postWhileBB);
/*
  %1 = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 0, i32* %1
  store i32 5, i32* %x, align 4
  br label %2

; <label>:2                                       ; preds = %5, %0
  %3 = load i32* %x, align 4
  %4 = icmp ne i32 %3, 0
  br i1 %4, label %5, label %7

; <label>:5                                       ; preds = %2
  %6 = load i32* %x, align 4
  br label %2

; <label>:7                                       ; preds = %2
  %8 = load i32* %x, align 4
  %9 = add nsw i32 %8, 1
  store i32 %9, i32* %x, align 4
  %10 = load i32* %x, align 4
  ret i32 %10

*/
}

llvm::Value* Function::genLLVM(GenLLVM* g){

	llvm::FunctionType *FT = &getFunctionType(*this);
	llvm::Function *F = static_cast<llvm::Function*>(g->getModule().getOrInsertFunction(getName(), FT));

	BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
	g->getBuilder().SetInsertPoint(BB);
	
	//Set names for the arguments and allocate them
	 //assert that number of symbols equal number of elements in typelist
	llvm::Function::arg_iterator argIter = F->arg_begin();
	for(std::list<Symbol*>::iterator symIter = getArgSymbolList().begin();
		argIter != F->arg_end(); ++argIter, ++symIter){
		argIter->setName((*symIter)->getName());

		//allocate a pointer to this variable
		llvm::AllocaInst* allocInst = g->getBuilder().CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0, "");
		g->getBuilder().CreateStore(argIter, allocInst);
		g->getNamedValues()[argIter->getName()] = allocInst;
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
}

GenLLVM::GenLLVM() : m_module(*new llvm::Module("MyModule", getGlobalContext())), m_irBuilder(*new llvm::IRBuilder<>(getGlobalContext())) {
}
