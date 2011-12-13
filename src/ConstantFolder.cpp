#include "ConstantFolder.h"

void ConstantFolder::Execute(Module& m){
	std::list<Function*>::iterator funcIter = m.getFunctions().begin();
	for(; funcIter != m.getFunctions().end(); ++funcIter)
		foldConstants(**funcIter);
}

void ConstantFolder::foldConstants(Function& f){
		
}


