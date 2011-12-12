#ifndef GENIL_H
#define GENIL_H

//#include "IClassVisitor.h" //just for the forward references
#include "codegen.h"
#include "ASTBuilder.h"

class GenIL {
public:
	GenIL(Module &m): m_module(m), m_astBuilder(*new ASTBuilder()), m_tempSeed(0) {}
	Module* generateIL();
	ASTBuilder& getBuilder() { return m_astBuilder; }
	Variable& getNextVariable();
private:
	Module& m_module;
	ASTBuilder& m_astBuilder;
	unsigned long m_tempSeed;
	//Prevent unintended C++ Synthesis
	GenIL();
};

#endif
