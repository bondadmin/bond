#include "bond/api/libruntime.h"
#include "bond/io/stdioinputstream.h"
#include "bond/io/stdiooutputstream.h"
#include "bond/io/stdiostreamfactory.h"
#include "bond/stl/vector.h"
#include "bond/systems/defaultallocator.h"
#include "bond/systems/exception.h"
#include "bond/vm/cboloader.h"
#include "bond/vm/codesegment.h"
#include "bond/vm/vm.h"
#include <cstdlib>
#include <cstring>

const size_t MIN_STACK_SIZE = 1;
const size_t DEFAULT_STACK_SIZE = 64;
const char *const DEFAULT_ENTRY_POINT = "main";
typedef Bond::Vector<const char *> StringList;
typedef Bond::Vector<Bond::StdioInputStream> InputStreamList;

int main(int argc, const char *argv[])
{
	Bond::ValidateConfiguration();

	Bond::DefaultAllocator allocator;
	StringList cboFileNameList((StringList::allocator_type(&allocator)));
	StringList argList((StringList::allocator_type(&allocator)));
	size_t stackSize = DEFAULT_STACK_SIZE;
	const char *entryPoint = DEFAULT_ENTRY_POINT;
	int32_t exitCode = 0;
	bool pushingApplicationArgs = false;
	bool error = false;

	cboFileNameList.reserve(size_t(argc));
	argList.reserve(size_t(argc));

	for (int i = 1; i < argc; ++i)
	{
		if (pushingApplicationArgs)
		{
			argList.push_back(argv[i]);
		}
		else if (strcmp(argv[i], "-") == 0)
		{
			pushingApplicationArgs = true;
		}
		else if (strcmp(argv[i], "-e") == 0)
		{
			if (++i < argc)
			{
				entryPoint = argv[i];
			}
			else
			{
				fprintf(stderr, "Missing argument to -e\n");
				error = true;
			}
		}
		else if (strcmp(argv[i], "-s") == 0)
		{
			if (++i < argc)
			{
				stackSize = size_t(atol(argv[i]));
				if (stackSize < MIN_STACK_SIZE)
				{
					fprintf(stderr, "Stack size must be at least %u kB.\n", unsigned(MIN_STACK_SIZE));
					error = true;
				}
			}
			else
			{
				fprintf(stderr, "Missing argument to -s\n");
				error = true;
			}
		}
		else if (argv[i][0] == '-')
		{
			fprintf(stderr, "Unknown option '%s'\n", argv[i]);
			error = true;
		}
		else
		{
			cboFileNameList.push_back(argv[i]);
		}
	}

	if (error)
	{
		return 1;
	}

	try
	{
		Bond::CodeSegmentHandle codeSegmentHandle;
		{
			Bond::CboLoader cboLoader(allocator);
			InputStreamList cboStreamList((InputStreamList::allocator_type(&allocator)));
			cboStreamList.reserve(cboFileNameList.size());

			Bond::LoadAllLibs(cboLoader);

			for (const auto &cboFileName: cboFileNameList)
			{
				cboStreamList.emplace_back(cboFileName);
				cboLoader.AddCboFile(cboStreamList.back());
			}

			codeSegmentHandle = cboLoader.Load();
		}

		const uint32_t numArgs = uint32_t(argList.size());
		const char **args = (numArgs > uint32_t(0)) ? &argList[0] : nullptr;
		Bond::StdInInputStream inStream;
		Bond::StdOutOutputStream outStream;
		Bond::StdErrOutputStream errStream;
		Bond::StdioStreamFactory streamFactory(allocator, nullptr, nullptr, false);
		Bond::VM vm(allocator, *codeSegmentHandle.get(), stackSize * 1024, &inStream, &outStream, &errStream, &streamFactory);
		codeSegmentHandle->CallStaticInitializers(vm);

		const Bond::Function *entryFunction = codeSegmentHandle->GetFunction(entryPoint);
		if (entryFunction != nullptr)
		{
			if (entryFunction->mParamListSignature.mParamCount == 0)
			{
				vm.CallFunction(*entryFunction, &exitCode);
			}
			else
			{
				vm.CallFunction(*entryFunction, &exitCode, numArgs, args);
			}
		}
		else
		{
			error = true;
			fprintf(stderr, "Failed to find entry point function: %s\n", entryPoint);
		}
	}
	catch (const Bond::Exception &e)
	{
		error = true;
		fprintf(stderr, "%s\n", e.GetMessage());
	}

	return error ? 1 : exitCode;
}
