#ifndef BOND_SEMANTICANALYZER_H
#define BOND_SEMANTICANALYZER_H

#include "bond/symboltable.h"

namespace Bond
{

class CompilerErrorBuffer;
class SymbolTable;

class SemanticAnalyzer
{
public:
	SemanticAnalyzer(CompilerErrorBuffer &errorBuffer, bu32_t pointerSize = BOND_NATIVE_POINTER_SIZE):
		mErrorBuffer(errorBuffer),
		mPointerSize(pointerSize)
	{}
	~SemanticAnalyzer() {}

	void Analyze(TranslationUnit *translationUnitList);

	const SymbolTable &GetSymbolTable() const { return mSymbolTable; }

private:
	SymbolTable mSymbolTable;
	CompilerErrorBuffer &mErrorBuffer;
	bu32_t mPointerSize;
};

}

#endif
