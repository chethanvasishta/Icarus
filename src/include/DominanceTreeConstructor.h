#ifndef DOMINANCE_TREE_CONSTRUCTOR
#define DOMINANCE_TREE_CONSTRUCTOR

#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/PassRegistry.h"
#include "DominanceTree.h"
#include <map>
using llvm::FunctionPass;
using llvm::BasicBlock;
struct DominanceTreeConstructor : public FunctionPass {
public:
    static char ID;
    DominanceTreeConstructor() : FunctionPass(ID){
       // ::initializeDominanceTreeConstructorPass(*llvm::PassRegistry::getPassRegistry());
    }

    virtual bool doInitialization(llvm::Module &M){}
	virtual bool runOnFunction(llvm::Function &F);
	virtual bool doFinalization(llvm::Module &M){}
    virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const {    AU.setPreservesAll();  } //we need to preserve the data we created

    std::map<BasicBlock*, BasicBlock*>& getDoms() { return doms; }

private:
    BasicBlock* intersect(BasicBlock*, BasicBlock*);

    std::map<BasicBlock*, BasicBlock*> doms; //These are nothing but immediate dominators for a given basic block
    std::map<BasicBlock*, int> uniq;

};//end of struct DominanceTreeConstructor

static llvm::RegisterPass<DominanceTreeConstructor> x("dominance", "dominance tree constructor", false /* true - read only pass */, true /* analysis pass*/);
#endif //DOMINANCE_TREE_CONSTRUCTOR

