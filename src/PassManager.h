#ifndef PASSMANAGER_H
#define PASSMANAGER_H

#include "ITransform.h"

class PassManager{
public:
	void addPass(ITransform& pass) { m_passes.push_back(&pass); }
	void run(Module&);
private:
	std::list<ITransform*> m_passes;
};

#endif //PASSMANAGER_H
