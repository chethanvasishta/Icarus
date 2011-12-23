#include "genIL.h"
#include <sstream>
#include <string>

Value* Constant::genIL(GenIL*){
	return new Constant(getValue());
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

Value* FunctionCall::genIL(GenIL* g){
	std::list<Value*> newParams;
	std::list<Value*>& oldParams = getParamList();
	std::list<Value*>::iterator paramIter = oldParams.begin();
	for(; paramIter != oldParams.end(); ++paramIter)
		newParams.push_back((*paramIter)->genIL(g));
	return new FunctionCall(getFunctionProtoType(), newParams);
}

Value* Assignment::genIL(GenIL* g){
	//LVal would be a variable, we just gen code
	Statement* stmt = new Assignment(getLVal(), *getRVal().genIL(g));
	g->getBuilder().insertStatement(*stmt);
	return stmt;
}

Value* ReturnStatement::genIL(GenIL* g){
	Statement* stmt = NULL;	
	if(getReturnValue() != NULL)
		stmt = new ReturnStatement(getReturnValue()->genIL(g));
	else 
		stmt = new ReturnStatement(NULL);
	g->getBuilder().insertStatement(*stmt);
	return stmt;
}

Value* WhileStatement::genIL(GenIL* g){
	Statement* stmt = new WhileStatement(*(Expression*)getCondition().genIL(g)); //TODO:this might be a problem. we might not evaluate expression everytime in the loop.
	g->getBuilder().insertStatement(*stmt);
	std::list<Statement*>::const_iterator iter = getStatements().begin();
	for(; iter != getStatements().end(); ++iter)
		(*iter)->genIL(g);
	g->getBuilder().endCodeBlock();
	return stmt;
}

Value* ExpressionStatement::genIL(GenIL* g){
	Statement* stmt = new ExpressionStatement(*((Expression*)getExpression().genIL(g)));
	g->getBuilder().insertStatement(*stmt);
	return stmt;
}

Value* BranchStatement::genIL(GenIL *g){
	ASTBuilder& builder = g->getBuilder();
	std::list<Branch*>::const_iterator branchIter = getBranches().begin();
	bool branchStatementAdded = false;
	BranchStatement *branchStmt = NULL;
	for(; branchIter != getBranches().end(); ++branchIter){
		Branch* branch = *branchIter;
		Expression& condition = *(Expression*) branch->getCondition().genIL(g);
		if(!branchStatementAdded){
			branchStmt = new BranchStatement(condition);
			builder.insertStatement(*branchStmt);
			branchStatementAdded = true;
		}
		else
			builder.addBranch(condition);
		std::list<Statement*>::const_iterator stmtIter = branch->getStatements().begin();
		for(; stmtIter != branch->getStatements().end(); ++stmtIter)
			(*stmtIter)->genIL(g);		
	}
	builder.endCodeBlock();
	return branchStmt;
}

Value *BreakStatement::genIL(GenIL* g){
	Statement* stmt = new BreakStatement();
	g->getBuilder().insertStatement(*stmt);
	return stmt;
}

Value* Function::genIL(GenIL* g){
	Function& funcRef = *(new Function(getProtoType(), getArgSymbolList()));
	g->getBuilder().addFunction(funcRef);
	std::list<Statement*>::const_iterator iter = getStatements().begin();
	for(; iter != getStatements().end(); ++iter){
		(*iter)->genIL(g);		
	}
	//we need to copy the symbols too;
	std::list<Symbol*>::iterator symIter = getSymbols().begin();
	for(; symIter != getSymbols().end(); ++symIter){		
		funcRef.addSymbol(*new Symbol(**symIter));
	}

}

Value* Module::genIL(GenIL* g){
	std::list<Function*>& funcList = getFunctions();
	for(std::list<Function*>::const_iterator funcIter = funcList.begin(); funcIter != funcList.end() ; ++funcIter){
		(*funcIter)->genIL(g);
	}
	//we need to copy the symbols too;
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
	Symbol* s = new Symbol(str, *new Type(Type::IntegerTy)); //Integer for temp
	m_astBuilder.addSymbol(*s);
	return *(new Variable(*s));
}
