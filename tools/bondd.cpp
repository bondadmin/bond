#include "bond/io/defaultfileloader.h"
#include "bond/io/stdouttextwriter.h"
#include "bond/systems/defaultallocator.h"
#include "bond/systems/exception.h"
#include "bond/tools/disassembler.h"
#include <stdio.h>

void Disassemble(const char *cboFileName)
{
	try
	{
		Bond::DefaultAllocator allocator;
		Bond::DefaultFileLoader fileLoader(allocator);
		Bond::FileLoader::Handle cboFileHandle = fileLoader.LoadFile(cboFileName);
		Bond::StdOutTextWriter writer;
		Bond::Disassembler disassembler(allocator);
		disassembler.Disassemble(writer, static_cast<const Bond::bu8_t *>(cboFileHandle.Get().mData), cboFileHandle.Get().mLength);
	}
	catch (const Bond::Exception &e)
	{
		fprintf(stderr, "%s\n", e.GetMessage());
	}
}


int main(int argc, const char *argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		Disassemble(argv[i]);
	}

	return 0;
}
