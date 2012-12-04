#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;
struct DominanceTreeConstructor : public FunctionPass {
public:
    static char ID;
    DominanceTreeConstructor() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F){
        errs() << "We are in Dominance Tree Constructor Pass - Chethan";
        return false;
    }

};//end of struct DominanceTreeConstructor

static RegisterPass<DominanceTreeConstructor> x("dominance", "dominance tree constructor", false /* true - read only pass */, true /* analysis pass*/);

