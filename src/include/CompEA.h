#ifndef COMPEA_H
#define COMPEA_H

class CompEA{
public:
	enum AddressingMode{
		kImmMode,
		kRegMode,
		kDispMode
	}; //sufficient for now :)
	CompEA(): m_disp(0), m_mode(kImmMode){}
	CompEA(int disp, AddressingMode mode): m_disp(disp), m_mode(mode){}

	//Getter-Setters
	int getDisp() { return m_disp; }
	AddressingMode getMode() { return m_mode; }
private:
	int m_disp;
	AddressingMode m_mode;
};

#endif //COMPEA_H
