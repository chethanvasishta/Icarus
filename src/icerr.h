#ifndef ICERR_H
#define ICERR_H

// a file to handle errors

enum IcErr{
	eNoErr = 0,
	eFunctionAlreadyDefined,
	eProtoTypeAlreadyDefined
};

extern std::string errMsg[];

#endif //ICERR_H
