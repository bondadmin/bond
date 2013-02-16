#include "framework/testlexerframework.h"
#include "bond/defaultallocator.h"
#include "bond/defaultfileloader.h"
#include "bond/exception.h"

namespace TestFramework
{

bool RunLexerTest(
	Bond::TextWriter &logger,
	const char *assertFile,
	int assertLine,
	const char *scriptName,
	LexerValidationFunction *validationFunction)
{
	__ASSERT_FORMAT__(scriptName != 0, logger, assertFile, assertLine, ("Script name is NULL."));
	__ASSERT_FORMAT__(validationFunction != 0, logger, assertFile, assertLine, ("Validation function is NULL."));

	Bond::DefaultAllocator fileLoaderAllocator;
	Bond::DefaultAllocator lexerAllocator;
	bool result = false;

	try
	{
		Bond::DefaultFileLoader fileLoader(fileLoaderAllocator);
		Bond::FileLoader::Handle scriptHandle = fileLoader.LoadFile(scriptName);
		Bond::CompilerErrorBuffer errorBuffer;
		Bond::Lexer lexer(lexerAllocator, errorBuffer);
		lexer.Lex(reinterpret_cast<const char *>(scriptHandle.Get().mData), scriptHandle.Get().mLength);
		result = validationFunction(logger, errorBuffer, lexer);
	}
	catch (const Bond::Exception &e)
	{
		logger.Write("line %u in %s: %s\n", assertLine, assertFile, e.GetMessage());
	}

	__ASSERT_FORMAT__(fileLoaderAllocator.GetNumAllocations() == 0, logger, assertFile, assertLine,
		("File loader leaked %d chunks of memory.", fileLoaderAllocator.GetNumAllocations()));
	__ASSERT_FORMAT__(lexerAllocator.GetNumAllocations() == 0, logger, assertFile, assertLine,
		("Lexer leaked %d chunks of memory.", lexerAllocator.GetNumAllocations()));

	return result;
}

}
