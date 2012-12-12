#ifndef DOMINANCE_FRONTIER
#define DOMINANCE_FRONTIER

#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
using llvm::FunctionPass;
struct DominanceFrontier : public FunctionPass {
public:
    static char ID;
    DominanceFrontier() : FunctionPass(ID){}

    virtual bool doInitialization(llvm::Module &M){}
	virtual bool runOnFunction(llvm::Function &F);
	virtual bool doFinalization(llvm::Module &M){}

};//end of struct DominanceFrontier

static llvm::RegisterPass<DominanceFrontier> x("dominance frontier", "dominance frontier calculator", false /* true - read only pass */, true /* analysis pass*/);
#endif //DOMINANCE_FRONTIER

