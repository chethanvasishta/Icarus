#include "debug.h"

Trace* Trace::m_instance = NULL;
Debug* Debug::m_instance = NULL;

Trace& Trace::getInstance(){
	if(m_instance == 0)
		m_instance = new Trace();
	return *m_instance;
}

Trace::~Trace(){
	if(m_instance != 0)
		delete m_instance;
}

Trace& Trace::operator<<(std::string s){
	if(Debug::getInstance().isTraceOn()){
		std::cout<<s;
	}
	return *this;
}

Debug::Debug(){
	m_isDebugOn = false;
	m_isTraceOn = false;
	m_showYaccTrace = false;
	m_isDotGen = false;
	m_isCodeOptimizing = false;
}

Debug& Debug::getInstance(){
	if(m_instance == 0)
		m_instance = new Debug();
	return *m_instance;
}
