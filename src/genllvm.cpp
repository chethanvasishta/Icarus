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
		case EQ:
			return builder.CreateICmpEQ(leftValue, rightValue, "");
			break;
		case NE:
			return builder.CreateICmpNE(leftValue, rightValue, "");
			break;
		case LT:
			return builder.CreateICmpULT(leftValue, rightValue, "");
			break;
		case LTEQ:
			return builder.CreateICmpULE(leftValue, rightValue, "");
			break;
		case GT:
			return builder.CreateICmpUGT(leftValue, rightValue, "");
			break;
		case GTEQ:
			return builder.CreateICmpUGE(leftValue, rightValue, "");
			break;		
		
	}
}

//Helper function to get a llvm function type from our function
llvm::FunctionType& getFunctionType(FunctionProtoType& fp, GenLLVM *g){
	std::vector<llvm::Type*> args;
	std::list<Type*>::iterator argTypeIter = fp.getTypeList().begin();
	for(; argTypeIter != fp.getTypeList().end(); ++argTypeIter){
		args.push_back(g->getLLVMType(**argTypeIter));
	}
	llvm::FunctionType *FT = llvm::FunctionType::get(g->getLLVMType(fp.getReturnType()), *new llvm::ArrayRef<llvm::Type*>(args), false);
	return *FT;
}

llvm::Value* FunctionCall::genLLVM(GenLLVM* g){
	std::vector<llvm::Value*> paramArrayRef;
	std::list<Value*> paramList = getParamList();
	std::list<Value*>::iterator paramIter = paramList.begin();	
	for(; paramIter != paramList.end(); ++paramIter){
		paramArrayRef.push_back((*paramIter)->genLLVM(g));
	}

	llvm::FunctionType *FT = &getFunctionType(getFunctionProtoType(), g);
	llvm::Function *F = static_cast<llvm::Function*>(g->getModule().getOrInsertFunction(getFunctionProtoType().getName(), FT));
	
	return g->getBuilder().CreateCall(F, *new llvm::ArrayRef<llvm::Value*>(paramArrayRef), "");
}

llvm::Value* Assignment::genLLVM(GenLLVM* g){
	return g->getBuilder().CreateStore(getRVal().genLLVM(g),g->getNamedValues()[getLVal().getSymbol().getName()], false);
}

llvm::Value* ReturnStatement::genLLVM(GenLLVM* g){
	if(getReturnValue() != NULL){
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
	llvm::Value* zeroConst = llvm::ConstantInt::get(getGlobalContext(), llvm::APInt(32 /*bits*/, 0 /*value*/, false /*isSigned*/));
	llvm::Type* conditionType = condition->getType();	
	if(!conditionType->isIntegerTy(1)) //bool type
		condition = builder.CreateICmpNE(condition, zeroConst, "");
	builder.CreateCondBr(condition, whileBodyBB, postWhileBB);

	builder.SetInsertPoint(whileBodyBB);
	
	std::list<Statement*>::iterator sIter = getStatements().begin();
	for(; sIter != getStatements().end(); ++sIter)
		(*sIter)->genLLVM(g);

	builder.CreateBr(whileBB); //loop back to while condition
	builder.SetInsertPoint(postWhileBB);
}

llvm::Value* BreakStatement::genLLVM(GenLLVM* g){
	
}

//Generation should be 
llvm::Value* BranchStatement::genLLVM(GenLLVM* g){

	llvm::IRBuilder<>& builder = g->getBuilder();	

	BasicBlock* curBlock = builder.GetInsertBlock();
	llvm::Function *func = curBlock->getParent();	
	
	llvm::Value* zeroConst = llvm::ConstantInt::get(getGlobalContext(), llvm::APInt(32 /*bits*/, 0 /*value*/, false /*isSigned*/));

	//Create basic blocks for each
	unsigned int size = getBranches().size();
	std::vector<BasicBlock*> basicBlocks(size*2+1);	
	for(unsigned int i = 0 ; i < size*2; i+=2){
		basicBlocks[i] = BasicBlock::Create(getGlobalContext(), "condblock", func);
		basicBlocks[i+1] = BasicBlock::Create(getGlobalContext(), "codeblock", func);
	}
	BasicBlock *postIfElseBB = BasicBlock::Create(getGlobalContext(),"postif", func);
	basicBlocks[size*2] = postIfElseBB;

	builder.CreateBr(basicBlocks[0]); //jump to first if condition
	
	std::list<Branch*>::const_iterator branchIter = getBranches().begin();
	unsigned int i = 0;
	for(; branchIter != getBranches().end(); ++branchIter, i+=2){
		Branch* branch = *branchIter;		

		builder.SetInsertPoint(basicBlocks[i]);	
		llvm::Value* condition = branch->getCondition().genLLVM(g);
		llvm::Type* conditionType = condition->getType();
		if(!conditionType->isIntegerTy(1)) //bool type
			condition = builder.CreateICmpNE(condition, zeroConst, "");		
		builder.CreateCondBr(condition, basicBlocks[i+1], basicBlocks[i+2]);
		builder.SetInsertPoint(basicBlocks[i+1]);
		
		std::list<Statement*> statements = branch->getStatements();
		std::list<Statement*>::const_iterator stmtIter = statements.begin();
		for(; stmtIter != statements.end(); ++stmtIter)
			(*stmtIter)->genLLVM(g);
		builder.CreateBr(postIfElseBB); //Jump to end of if-else after completing this codeblock
	}
	builder.SetInsertPoint(postIfElseBB);
}

llvm::Value* Function::genLLVM(GenLLVM* g){

	llvm::FunctionType *FT = &getFunctionType(getProtoType(), g);
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
		llvm::AllocaInst* allocInst = g->getBuilder().CreateAlloca(g->getLLVMType((*symIter)->getType()), 0, "");
		g->getBuilder().CreateStore(argIter, allocInst);
		g->getNamedValues()[argIter->getName()] = allocInst;
	}

	//assume only the local symbols are visited now
	std::list<Symbol*>::iterator symIter = getSymbols().begin();
	for(; symIter != getSymbols().end(); ++symIter){
		g->getNamedValues()[(*symIter)->getName()] = g->getBuilder().CreateAlloca(g->getLLVMType((*symIter)->getType()), 0, (*symIter)->getName());
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

//Helpers

llvm::Type* GenLLVM::getLLVMType(Type& type){
	switch(type.getTypeID()){
		case Type::IntegerTy:
			return llvm::Type::getInt32Ty(getGlobalContext());
		case Type::FloatTy:
			return llvm::Type::getFloatTy(getGlobalContext());
		case Type::VoidTy:
			return llvm::Type::getVoidTy(getGlobalContext());
	
	}
}
