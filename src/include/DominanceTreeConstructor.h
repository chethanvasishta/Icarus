#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
using llvm::FunctionPass;
struct DominanceTreeConstructor : public FunctionPass {
public:
    static char ID;
    DominanceTreeConstructor() : FunctionPass(ID) {}

    virtual bool runOnFunction(llvm::Function &F){
        llvm::errs() << "We are in Dominance Tree Constructor Pass - Chethan";
        return false;
    }

};//end of struct DominanceTreeConstructor

static llvm::RegisterPass<DominanceTreeConstructor> x("dominance", "dominance tree constructor", false /* true - read only pass */, true /* analysis pass*/);

