/* We have a LLVM IR that we need to construct a Dominance Tree for */
#include "DominanceTreeConstructor.h"
#include "debug.h"
#include <map>
#include <set>
//#include <densemap.h>
//
using llvm::BasicBlock;
using namespace std;
char DominanceTreeConstructor::ID = 0;

static Trace& gTrace = Trace::getInstance();

bool DominanceTreeConstructor::runOnFunction(llvm::Function &F){
	gTrace<<" Constructing Dominator Tree for "<<F.getName();

    llvm::Function::BasicBlockListType& bbList = F.getBasicBlockList();
    llvm::Function::BasicBlockListType::iterator iter = bbList.begin();
    /*
     * Create a multimap of nodes mapped to their dominators.
     * Set the dominator of each node to be itself
     * iterate through the basic blocks and re calculate the dominators for each of them
     * Check if any of the dominator sets changed. If so repeat the process
     */
    int numBlocks = F.getBasicBlockList().size();
    std::map<BasicBlock*, std::set<BasicBlock*>* > dom;
    for(int i = 0 ; i < numBlocks ; ++i, ++iter){
        std::set<BasicBlock*> *domSet = new std::set<BasicBlock*>(); //we cannot allocate on the stack!
        domSet->insert(&*iter);
        dom.insert(pair<BasicBlock*, std::set<BasicBlock*>*>(&*iter, domSet));
    }
    bool changed = true;

    while(changed){
        changed = false;
        for(iter = bbList.begin() ; iter != F.getBasicBlockList().end(); ++iter){
            std::set<BasicBlock*> *domSet = dom[&*iter];
            int numElts = domSet->size();
            for(int i = 0 ; i < numElts ; ++i){
                
            }
            
            if(domSet->size() != numElts)
                changed = true;
                        
        }
    }

	return false;
}
