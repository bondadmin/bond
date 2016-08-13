#ifndef BOND_COMPILER_LEXER_H
#define BOND_COMPILER_LEXER_H

#include "bond/compiler/tokenstream.h"
#include "bond/systems/allocator.h"

namespace Bond
{

class Allocator;
class CharStream;
class CompilerErrorBuffer;
class StringAllocator;

/// \brief A smart pointer to a TokenCollection.
typedef Allocator::Handle<const TokenCollection> TokenCollectionHandle;

/// \brief A lexer for the Bond scripting language.
///
/// The Lexer tokenizes Bond source code passed in as a C-style string. Its output is a
/// TokenCollection that can be used as input to a Parser. A single Lexer can be used to tokenize
/// many strings of Bond source code; each invocation generates a new TokenCollection.
///
/// When writing a tool that requires a compiler front end, the interactions between the front end
/// components, namely a Lexer, a Parser and a SemanticAnalyzer, can be managed by a FrontEnd.
///
/// \sa CompilerErrorBuffer, FrontEnd, Parser, TokenCollection
/// \ingroup compiler
class Lexer
{
public:
	/// \brief Constructs a Lexer object.
	/// \param allocator The memory allocator from which TokenCollections are allocated.
	/// \param errorBuffer Buffer where error messages are pushed when invalid tokens are encountered.
	Lexer(Allocator &allocator, CompilerErrorBuffer &errorBuffer):
		mAllocator(allocator),
		mErrorBuffer(errorBuffer)
	{}

	/// \brief Scans the given string of Bond source and breaks it up into a sequence of Tokens.
	/// \param fileName Name to associate with the string of Bond source code, ideally the name of the
	///        file from which the code was loaded. The name will be accessible via the generated
	///        Tokens, so that other tools, such as a Parser or SemanticAnalyzer can generate
	///        meaningful error messages. The name is not copied so the given pointer must remain
	///        valid for the lifetime of the generated TokenCollection.
	/// \param text The text for the Bond source code to be tokenized. The tokens contain a copy
	///        of the original source code, so the given pointer does not need to remain valid once
	///        this function returns.
	/// \param length The number of characters in the Bond source code. Required since the source
	///        code may contain null characters and may not be null terminated.
	/// \returns A TokenCollectionHandle, which is an owning pointer to a dynamically allocated
	///        TokenCollection. The TokenCollection is automatically destroyed when the handle
	///        is destroyed.
	TokenCollectionHandle Lex(const char *fileName, const char *text, size_t length);

	/// \brief Returns the buffer where error messages are pushed.
	const CompilerErrorBuffer &GetErrorBuffer() const { return mErrorBuffer; }

private:
	// Copying disallowed.
	Lexer(const Lexer &other) = delete;
	Lexer &operator=(const Lexer &other) = delete;

	Allocator &mAllocator;
	CompilerErrorBuffer &mErrorBuffer;
};

}

#endif
