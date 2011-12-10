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
	bool setDebug(bool isOn) { m_isDebugOn = isOn; }
	bool isDebuggable() { return m_isDebugOn; }
	
	static Debug& getInstance();
private:
	bool m_isDebugOn;
	static Debug* m_instance;
};

#endif //DEBUG_H
