#ifndef DOMINANCE_TREE_CONSTRUCTOR
#define DOMINANCE_TREE_CONSTRUCTOR

#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
using llvm::FunctionPass;
struct DominanceTreeConstructor : public FunctionPass {
public:
    static char ID;
    DominanceTreeConstructor() : FunctionPass(ID){}

    virtual bool doInitialization(llvm::Module &M){}
	virtual bool runOnFunction(llvm::Function &F);
	virtual bool doFinalization(llvm::Module &M){}

};//end of struct DominanceTreeConstructor

static llvm::RegisterPass<DominanceTreeConstructor> x("dominance", "dominance tree constructor", false /* true - read only pass */, true /* analysis pass*/);
#endif //DOMINANCE_TREE_CONSTRUCTOR

