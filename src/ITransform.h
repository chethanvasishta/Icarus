#ifndef ITRANSFORM_H
#define ITRANSFORM_H

#include "codegen.h"

class ITransform {
public:
	virtual void Execute(Module& ) = 0;
};

#endif //ITRANSFORM_H
