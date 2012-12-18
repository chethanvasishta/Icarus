#ifndef DOMINANCE_FRONTIER
#define DOMINANCE_FRONTIER

#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <set>
#include <map>
#include "DominanceTreeConstructor.h"

using llvm::FunctionPass;
using llvm::BasicBlock;

struct DominanceFrontier : public FunctionPass {
public:
    static char ID;
    DominanceFrontier() : FunctionPass(ID){}

    virtual bool doInitialization(llvm::Module &M){}
	virtual bool runOnFunction(llvm::Function &F);
	virtual bool doFinalization(llvm::Module &M){}

    virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const {
        AU.setPreservesAll();
        AU.addRequired<DominanceTreeConstructor>();
    }

private:
    std::map<BasicBlock*, std::set<BasicBlock*> > m_frontiers;

    //helper functions
    void print();

};//end of struct DominanceFrontier

static llvm::RegisterPass<DominanceFrontier> df("dominance frontier", "dominance frontier calculator", false /* true - read only pass */, true /* analysis pass*/);
#endif //DOMINANCE_FRONTIER

