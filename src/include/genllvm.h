#ifndef GENLLVM_H
#define GENLLVM_H

#include "IClassVisitor.h"
#include <llvm/Support/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <map>
#include <string>
#include "Type.h"

class GenLLVM {
public:
	GenLLVM();	
	void generateLLVM(Module &);
	llvm::IRBuilder<>& getBuilder() { return m_irBuilder; }
	llvm::Module& getModule() { return m_module; }
	std::map<std::string, llvm::Value*>& getNamedValues() { return m_namedValues; }
	llvm::Type* getLLVMType(Type& type);
	
private:
	llvm::IRBuilder<>& m_irBuilder;
	llvm::Module& m_module;	
	std::map<std::string, llvm::Value*> m_namedValues; //to hold the temp allocations in the function
};

#endif //GENLLVM_H
