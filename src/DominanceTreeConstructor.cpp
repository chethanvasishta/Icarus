/* We have a LLVM IR that we need to construct a Dominance Tree for */
#include "DominanceTreeConstructor.h"
#include "debug.h"
#include <map>
#include <set>
#include <llvm/ADT/DepthFirstIterator.h> //For iterating through the predecessors of the basic block
#include <llvm/Support/CFG.h> //We need a specialization of the GraphTrait class 

using namespace std;
using namespace llvm;
char DominanceTreeConstructor::ID = 0;

static Trace& gTrace = Trace::getInstance();

bool DominanceTreeConstructor::runOnFunction(Function &F){
	gTrace<<" Constructing Dominator Tree for "<<F.getName();

    Function::BasicBlockListType& bbList = F.getBasicBlockList();
    Function::BasicBlockListType::iterator iter = bbList.begin();
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
        for(Function::iterator bbIter = F.begin() ; bbIter != F.end(); ++bbIter){ //for each basic block 
                std::set<BasicBlock*> newDom;
                BasicBlock* bb = dynamic_cast<BasicBlock*>(&*bbIter);
                for(idf_iterator<BasicBlock*> predIter = idf_begin(bb); //for each of its predecessor
                                              predIter != idf_end(bb); 
                                              ++predIter){
                    std::set<BasicBlock*> *predDom = dom[*predIter];
                    for(std::set<BasicBlock*>::iterator predDomIter = predDom->begin() ; predDomIter != predDom->end(); ++predDomIter){
                        newDom.insert(*predDomIter);
                    }
                }
                newDom.insert(bb); //Union n
                if(newDom != *dom[bb])
                    changed = true;
        }
    }
	return false;
}
