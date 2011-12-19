#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <fstream>
using namespace std;

//Singleton debug class
class Trace{
public: 
	static Trace& getInstance();
	~Trace();

	Trace& operator<<(std::string s);
	//Trace& operator<<(char* s);
private:
	static Trace* m_instance;
	Trace(){}
};

class Debug {
public:
	void setDebug(bool isOn) { m_isDebugOn = isOn; }
	bool isDebuggable() { return m_isDebugOn; }

	void setTrace(bool isOn) { m_isTraceOn = isOn; }
	bool isTraceOn() { return m_isTraceOn; } 

	void setYaccTrace(bool isOn) { m_showYaccTrace = isOn; }
	bool isYaccTraceOn() { return m_showYaccTrace; }
	
	void setDotGen(bool isOn) { m_isDotGen = isOn; }
	bool isDotGen() { return m_isDotGen; }

	void setCodeOptimization(bool isOn) { m_isCodeOptimizing = isOn; }
	bool isOptimizing() { return m_isCodeOptimizing; }
	
	static Debug& getInstance();
private:
	bool m_isDebugOn;
	bool m_isTraceOn;
	bool m_showYaccTrace;
	bool m_isDotGen;
	bool m_isCodeOptimizing;
	static Debug* m_instance;
	Debug();
};

#endif //DEBUG_H
