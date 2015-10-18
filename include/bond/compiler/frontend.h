#ifndef BOND_COMPILER_FRONTEND_H
#define BOND_COMPILER_FRONTEND_H

#include "bond/stl/list.h"
#include "bond/types/simplestring.h"

namespace Bond
{

class Allocator;
class FileLoader;
class Lexer;
class Parser;
class SemanticAnalyzer;

/// \brief A compiler front end for the Bond scripting language.
///
/// The FrontEnd manages the interactions between the compiler's front end components, namely a
/// Lexer, a Parser and a SemanticAnalyzer.
///
/// The FrontEnd also manages an input file list, a list of Bond source files that are required to
/// be loaded, parsed and analyzed. Files can be explicitly added to the input file list via the
/// AddInputFile method. Files are also implicitly added to the list when include directives in
/// the Bond source files are parsed. A source file is never added to the input file list more
/// than once, so encoutering an include directive for a file that already exists in the list has
/// no effect. One parse tree, rooted with a TranslationUnit, is generated for each Bond source
/// file. Only the TranslationUnits for the files that were explicitly added to the input file
/// list are flagged as requiring code generation.
///
/// When analysis is complete, the list of TranslationUnits
///
/// File loading is delegated an object that implements the FileLoader interface so that no
/// assumptions about where the source files reside are made (e.g. disk, network, database,
/// proprietary asset store, etc).
///
/// \sa CompilerErrorBuffer, FrontEnd, Parser, SemanticAnalyzer, TranslationUnit
class FrontEnd
{
public:
	/// \brief Constructs a FrontEnd object.
	/// \param allocator The allocator used for the FrontEnd's internal data structures.
	/// \param lexer The Lexer used to scan the Bond source files.
	/// \param parser The Parser used to parse the Bond source files.
	/// \param semanticAnalyzer The SemanticAnalyzer used to analyze the Bond source files.
	/// \param fileLoader The FileLoader responsible for loading the Bond source files.
	FrontEnd(
			Allocator &allocator,
			Lexer &lexer,
			Parser &parser,
			SemanticAnalyzer &semanticAnalyzer,
			FileLoader &fileLoader):
		mInputFileNameList(StringList::Allocator(&allocator)),
		mLexer(lexer),
		mParser(parser),
		mSemanticAnalyzer(semanticAnalyzer),
		mFileLoader(fileLoader)
	{}

	/// \brief Adds the name of a Bond source file to the input file list.
	/// \param inputFileName The name of the file to be loaded. Since SimpleStrings are not deep
	/// copied, the lifetime of the string must at last span the lifetime of the FrontEnd.
	void AddInputFile(const SimpleString &inputFileName);

	/// \brief Returns whether the given file name exists in the input file list..
	/// \param inputFileName The name of the file to be tested.
	bool ContainsInputFile(const SimpleString &inputFileName);

	/// \brief Performs loading, parsing and semantic analysis of all source files in the input file
	/// list, as well as those files referenced via include directives.
	void Analyze();

	/// \brief Returns whether any errors occured whilst anylizing the source code.
	bool HasErrors() const;

private:
	FrontEnd(const FrontEnd &other) = delete;
	FrontEnd &operator=(const FrontEnd &other) = delete;

	typedef List<SimpleString> StringList;

	StringList::Type mInputFileNameList;
	Lexer &mLexer;
	Parser &mParser;
	SemanticAnalyzer &mSemanticAnalyzer;
	FileLoader &mFileLoader;
};

}

#endif
