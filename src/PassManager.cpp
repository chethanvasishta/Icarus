#include "PassManager.h"

void PassManager::run(Module& m){
	std::list<ITransform*>::iterator passIter = m_passes.begin();
	for(; passIter != m_passes.end(); ++passIter)
		(*passIter)->Execute(m);
}
