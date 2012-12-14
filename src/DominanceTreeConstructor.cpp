/* We have a LLVM IR that we need to construct a Dominance Tree for */
#include "DominanceTreeConstructor.h"
#include "debug.h"
#include <map>
#include <set>
#include <llvm/ADT/DepthFirstIterator.h> //For iterating through the predecessors of the basic block
#include <llvm/ADT/PostOrderIterator.h> //For iterating through the predecessors of the basic block
#include <llvm/Support/CFG.h> //We need a specialization of the GraphTrait class 
#include "DominanceTree.h"

using namespace std;
//using namespace llvm;
using llvm::BasicBlock;
using llvm::Function;
using namespace llvm;
char DominanceTreeConstructor::ID = 0;

static Trace& gTrace = Trace::getInstance();

bool DominanceTreeConstructor::runOnFunction(llvm::Function &F){
	gTrace<<" Constructing Dominator Tree for "<<F.getName(); 
    /*
     * Create a multimap of nodes mapped to their dominators.
     * Set the dominator of each node to be itself
     * iterate through the basic blocks and re calculate the dominators for each of them
     * Check if any of the dominator sets changed. If so repeat the process
     */
    /* Function::BasicBlockListType& bbList = F.getBasicBlockList();
    std::map<BasicBlock*, std::set<BasicBlock*>* > dom;
    for(Function::BasicBlockListType::iterator iter = bbList.begin(); iter != bbList.end() ;++iter){
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
                if(newDom != *dom[bb]){
                    changed = true;
                    *dom[bb] = newDom;
                }
        }
    } */

    // Keith D. Cooper's algorithm for computing dominators
    
    ::DominatorTree<BasicBlock> domTree;
    std::map<BasicBlock*, BasicBlock*> doms; //These are nothing but immediate dominators for a given basic block
    
    Function::BasicBlockListType& bbList = F.getBasicBlockList();
    for(Function::BasicBlockListType::iterator iter = bbList.begin(); iter != bbList.end() ;++iter){
        //Set everything to undefined - NULL
        doms[&*iter] = NULL;
    }

    BasicBlock* startNode = &F.getEntryBlock();
    doms[startNode] = startNode;
    domTree.setRoot(startNode); //can we make this more generic?

    bool changed = true;
    while(changed){
        changed = false;
        ReversePostOrderTraversal<Function*> RPOT(&F);
        for(ReversePostOrderTraversal<Function*>::rpo_iterator rpoIter = RPOT.begin(); rpoIter != RPOT.end(); ++rpoIter){
            if(*rpoIter != startNode){
                BasicBlock* newIdom = NULL;//first processed predecessor of rpoIter
                for(idf_iterator<BasicBlock*> predIter = idf_begin(*rpoIter); //for each of its predecessor
                                              predIter != idf_end(*rpoIter); 
                                              ++predIter){
                    BasicBlock* pred = *predIter;
                    if(doms[pred] != NULL){
                        if(newIdom == NULL)
                            newIdom = pred;
                        else
                            newIdom = intersect(pred, newIdom);
                    }
                }
                if(newIdom != doms[*rpoIter]){
                    doms[*rpoIter] = newIdom;
                    changed = true;
                }
            }
        }
    }
	return false;
}
