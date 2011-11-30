#include "genIL.h"
#include <sstream>
#include <string>

Value* Constant::genIL(GenIL*){

}

Value* Variable::genIL(GenIL* g){
	return new Variable(getSymbol());
}

Value* BinopExpression::genIL(GenIL* g){
	//of lval or rval is a binop, generate an assignment statement and insert
	Value *lVal = NULL;
	if(dynamic_cast<BinopExpression*>(&getLeftValue())){ // we need to check for function calls etc
		Variable& nextVarRef = g->getNextVariable();
		Assignment* tempAssign = new Assignment(nextVarRef, *getLeftValue().genIL(g));
		g->getBuilder().insertStatement(*tempAssign);
		lVal = &nextVarRef;
	}
	else
		lVal = &getLeftValue();
	
	Value *rVal = NULL;
	if(dynamic_cast<BinopExpression*>(&getRightValue())){ // we need to check for function calls etc
		Variable& nextVarRef = g->getNextVariable();
		Assignment* tempAssign = new Assignment(nextVarRef, *getRightValue().genIL(g));
		g->getBuilder().insertStatement(*tempAssign);
		rVal = &nextVarRef;
	}
	else
		rVal = &getRightValue();
	return new BinopExpression(*lVal, *rVal, getOperation());
}

Value* FunctionCall::genIL(GenIL*){

}

Value* Assignment::genIL(GenIL* g){
	//LVal would be a variable, we just gen code
	return new Assignment(getLVal(), *getRVal().genIL(g));
}

Value* ReturnStatement::genIL(GenIL* g){
	if(getReturnValue() != NULL)
		return new ReturnStatement(getReturnValue()->genIL(g));
	return new ReturnStatement(NULL);
}

Value* ExpressionStatement::genIL(GenIL*){

}

Value* Function::genIL(GenIL* g){
	Function& funcRef = *(new Function(getName(), getProtoType(), getArgSymbolList()));
	g->getBuilder().addFunction(funcRef);
	std::list<Statement*>::iterator iter = getStatements().begin();
	for(; iter != getStatements().end(); ++iter){
		Statement *stmt = (Statement*)(*iter)->genIL(g);
		g->getBuilder().insertStatement(*stmt);
	}
}

Value* Module::genIL(GenIL* g){
	std::list<Function*>& funcList = getFunctions();	
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		(*funcIter)->genIL(g);
	}
	return NULL; //we wont use it anyway. This function should actually return nothing
}

Module* GenIL::generateIL(){
	m_module.genIL(this);
	return &(m_astBuilder.getModule());
}

Variable& GenIL::getNextVariable(){
	std::ostringstream os;
	os << "_M_"<<m_tempSeed++;
	std::string str = os.str();
	Symbol* s = new Symbol(str);
	m_astBuilder.addSymbol(*s);
	return *(new Variable(*s));
}
