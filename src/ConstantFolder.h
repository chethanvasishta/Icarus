#ifndef CONSTANT_FOLDER_H
#define CONSTANT_FOLDER_H

//#include "ITransform.h"
#include <llvm/Pass.h>
#include <llvm/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "codegen.h"
using llvm::FunctionPass;

class ConstantFolder : public FunctionPass {
public:
	static char ID;
	ConstantFolder() : FunctionPass(ID) {}

	virtual bool doInitialization(llvm::Module &M){}
	virtual bool runOnFunction(llvm::Function &F);
	virtual bool doFinalization(llvm::Module &M){}
};

#endif //CONSTANT_FOLDER_H
