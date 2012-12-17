/* We have a LLVM IR that we need to construct a Dominance Tree for */
#include "DominanceTreeConstructor.h"
#include "debug.h"
#include <map>
#include <set>
#include <queue>
#include <llvm/ADT/DepthFirstIterator.h> //For iterating through the predecessors of the basic block
#include <llvm/ADT/PostOrderIterator.h> //For iterating through the predecessors of the basic block
#include <llvm/Support/CFG.h> //We need a specialization of the GraphTrait class 
#include "DominanceTree.h"

using llvm::BasicBlock;
using llvm::Function;
using namespace llvm;
char DominanceTreeConstructor::ID = 0;

INITIALIZE_PASS(DominanceTreeConstructor, "domtree", "Dominator Tree Construction", true, true)

static Trace& gTrace = Trace::getInstance();

bool DominanceTreeConstructor::runOnFunction(llvm::Function &F){
	gTrace<<" Constructing Dominator Tree for "<<F.getName(); 
   
    // Keith D. Cooper's algorithm for computing dominators
    
    DominanceTree<BasicBlock> domTree;
    
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
        llvm::ReversePostOrderTraversal<Function*> RPOT(&F);
        for(llvm::ReversePostOrderTraversal<Function*>::rpo_iterator rpoIter = RPOT.begin(); rpoIter != RPOT.end(); ++rpoIter){
            if(*rpoIter != startNode){
                BasicBlock* newIdom = NULL;//first processed predecessor of rpoIter
                for(llvm::idf_iterator<BasicBlock*> predIter = idf_begin(*rpoIter); //for each of its predecessor
                                              predIter != idf_end(*rpoIter); 
                                              ++predIter){
                    BasicBlock* pred = *predIter;
                    if(doms[pred] != NULL){
                        if(newIdom == NULL)
                            newIdom = pred;
                        else
                            newIdom = intersect(domTree, doms, pred, newIdom);
                    }
                }
                if(newIdom != doms[*rpoIter]){
                    domTree.setIDom(*rpoIter, newIdom);
                    doms[*rpoIter] = newIdom;
                    changed = true;
                }
            }
        }
    }

    //if debug
    domTree.dump();
	return false;
}

BasicBlock* DominanceTreeConstructor::intersect(DominanceTree<BasicBlock>& domTree, std::map<BasicBlock*, BasicBlock*>& doms, BasicBlock* pred, BasicBlock* newIdom){
    BasicBlock* finger1 = pred;
    BasicBlock* finger2 = newIdom;
    while(domTree.depth(finger1) != domTree.depth(finger2)){
        while(domTree.depth(finger1) > domTree.depth(finger2))
            finger1 = doms[finger1];
        while(domTree.depth(finger2) > domTree.depth(finger1))
            finger2 = doms[finger2];
    }
    return finger1; //for now
}

template<>
void DominanceTree<BasicBlock>::dump(){
    gTrace<<"Printing dominance tree\n";
    if(m_root == NULL){
        gTrace<<"Graph is empty\n";
        return;
    }
    std::queue<DominanceNode<BasicBlock>* > visitQ;
    visitQ.push(m_root);
    while(!visitQ.empty()){
        DominanceNode<BasicBlock>* node = visitQ.front();
        gTrace<<"*"<<node->getActualNode()->getName()<<" --> ";
        visitQ.pop();
        std::set<DominanceNode<BasicBlock>*>& children = node->getChildren();
        for(typename std::set<DominanceNode<BasicBlock>*>::iterator iter = children.begin();
                                          iter != children.end(); ++iter){
            gTrace<<((DominanceNode<BasicBlock>*)(*iter))->getActualNode()->getName()<<" ";
            visitQ.push(*iter);
        }
        gTrace<<"\n";
    }
}

