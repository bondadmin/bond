#ifndef BOND_STDIOTEXTWRITER_H
#define BOND_STDIOTEXTWRITER_H

#include "bond/io/textwriter.h"
#include <stdio.h>

namespace Bond
{

class StdioTextWriter: public TextWriter
{
public:
	StdioTextWriter(FILE *file): mFile(file) {}
	virtual ~StdioTextWriter() {}
	virtual void Write(const char *format, ...);

private:
	FILE *mFile;
};


class StdOutTextWriter: public StdioTextWriter
{
public:
	StdOutTextWriter(): StdioTextWriter(stdout) {}
	virtual ~StdOutTextWriter() {}
};


class StdErrTextWriter: public StdioTextWriter
{
public:
	StdErrTextWriter(): StdioTextWriter(stderr) {}
	virtual ~StdErrTextWriter() {}
};

}

#endif
