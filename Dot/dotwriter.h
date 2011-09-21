#ifndef DOTWRITER_H
#define DOTWRITER_H
#include "../codegen.h"
class DotWriter{
	static void writeDotFile(char *filename, const Module& m);
};
#endif
