/*
 * Calculates the dominance frontiers for each basic block in a given function
 * The algorithm is based on the "The Simple, Fast Dominance Algorithm - Keith D. Cooper, Timothy J. Harvey and Ken Kennedy
 * http://www.hipersoft.rice.edu/grads/publications/dom14.pdf
 */

#include "DominanceFrontier.h"
#include "debug.h"
#include <map>
#include <set>
#include <llvm/ADT/DepthFirstIterator.h> //For iterating through the predecessors of the basic block
#include <llvm/Support/CFG.h> //We need a specialization of the GraphTrait class 

using namespace std;
using namespace llvm;
char DominanceFrontier::ID = 0;

static Trace& gTrace = Trace::getInstance();

bool DominanceFrontier::runOnFunction(Function &F){
	gTrace<<" Calculating Dominator Frontier for "<<F.getName(); 

	return false;
}
