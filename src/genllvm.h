#ifndef GENLLVM_H
#define GENLLVM_H

#include "IClassVisitor.h"
#include <llvm/Support/IRBuilder.h>
#include <llvm/LLVMContext.h>

namespace llvm{

class GenLLVM : IClassVisitor {
public:
//	GenLLVM() : m_irBuilder(*new IRBuilder(getGlobalContext())){}
	virtual void Visit(::Value& );
	virtual void Visit(::Expression&);
	virtual void Visit(::Variable&);
	virtual void Visit(::BinopExpression&);
	virtual void Visit(::FunctionCall&);
	virtual void Visit(::Statement&);
	virtual void Visit(::ExpressionStatement&);	
	virtual void Visit(::Assignment&);
	virtual void Visit(::ReturnStatement&);
	virtual void Visit(::FunctionProtoType&);
	virtual void Visit(::Function&);
	virtual void Visit(::SymbolTable&);	
	virtual void Visit(::Symbol& );
	virtual void Visit(::Module& );
	
	void generateLLVM(::Module &);
private:
	IRBuilder<> m_irBuilder;
};

}

#endif //GENLLVM_H
