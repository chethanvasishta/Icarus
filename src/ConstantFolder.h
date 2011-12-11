#ifndef CONSTANT_FOLDER_H
#define CONSTANT_FOLDER_H

#include "ITransform.h"

class ConstantFolder : public ITransform {
public:
	virtual void Execute(Module& m);
private:
	void foldConstants(Function& f);
};

#endif //CONSTANT_FOLDER_H
