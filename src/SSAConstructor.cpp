#include "SSAConstructor.h"
using llvm::Function;
char SSAConstructor::ID = 0;

bool SSAConstructor::runOnFunction(Function& F){
    return false;
}
