#include "bond/codegenerator.h"
#include "bond/compilererror.h"
#include "bond/defaultallocator.h"
#include "bond/defaultfileloader.h"
#include "bond/exception.h"
#include "bond/filebinarywriter.h"
#include "bond/lexer.h"
#include "bond/parser.h"
#include "bond/semanticanalyzer.h"
#include "bond/stdouttextwriter.h"
#include <stdio.h>

void PrintErrors(Bond::TextWriter &writer, const Bond::CompilerErrorBuffer &errorBuffer)
{
	if (errorBuffer.HasErrors())
	{
		for (size_t i = 0; i < errorBuffer.GetNumErrors(); ++i)
		{
			const Bond::CompilerError &error = errorBuffer.GetError(i);
			error.Print(writer);
			writer.Write("\n");
		}
	}
}


void Compile(const char *scriptName)
{
	try
	{
		Bond::DefaultAllocator allocator;
		Bond::DefaultFileLoader fileLoader(allocator);
		Bond::FileLoader::Handle scriptHandle = fileLoader.LoadFile(scriptName);
		Bond::CompilerErrorBuffer errorBuffer;
		Bond::Lexer lexer(allocator, errorBuffer);
		lexer.Lex(reinterpret_cast<const char *>(scriptHandle.Get().mData), scriptHandle.Get().mLength);

		Bond::Parser parser(allocator, errorBuffer);
		if (!errorBuffer.HasErrors())
		{
			Bond::TokenStream stream = lexer.GetTokenCollectionList()->GetTokenStream();
			parser.Parse(stream);
		}

		if (!errorBuffer.HasErrors())
		{
			Bond::SemanticAnalyzer analyzer(errorBuffer);
			analyzer.Analyze(parser.GetTranslationUnitList());

			if (!errorBuffer.HasErrors())
			{
				FILE *outputFile = fopen("bond.cbo", "wb");
				if (outputFile != NULL)
				{
					Bond::FileBinaryWriter cboWriter(outputFile);
					Bond::CodeGenerator generator(allocator, errorBuffer);
					generator.Generate(parser.GetTranslationUnitList(), cboWriter);
					fclose(outputFile);
				}
			}
		}

		Bond::StdOutTextWriter errorWriter;
		PrintErrors(errorWriter, errorBuffer);
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
		Compile(argv[i]);
	}

	return 0;
}
