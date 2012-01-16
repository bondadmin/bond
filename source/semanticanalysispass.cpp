#include "private/semanticanalysispass.h"
#include "bond/parseerror.h"
#include "bond/symboltable.h"

namespace Bond
{

void SemanticAnalysisPass::Analyze(TranslationUnit *translationUnitList)
{
	TraverseList(translationUnitList);
}


void SemanticAnalysisPass::Visit(TranslationUnit *translationUnit)
{
	ScopeStack::Element globalScopeElement(mScopeStack, mSymbolTable.GetGlobalScope());
	ParseNodeTraverser::Visit(translationUnit);
}


void SemanticAnalysisPass::Visit(NamespaceDefinition *namespaceDefinition)
{
	ScopeStack::Element stackElement(mScopeStack, namespaceDefinition);
	ParseNodeTraverser::Visit(namespaceDefinition);
}


void SemanticAnalysisPass::Visit(StructDeclaration *structDeclaration)
{
	ScopeStack::Element stackElement(mScopeStack, structDeclaration);
	ParseNodeTraverser::Visit(structDeclaration);
}


void SemanticAnalysisPass::Visit(FunctionDefinition *functionDefinition)
{
	ScopeStack::Element stackElement(mScopeStack, functionDefinition);
	ParseNodeTraverser::Visit(functionDefinition);
}


void SemanticAnalysisPass::Visit(CompoundStatement *compoundStatement)
{
	ScopeStack::Element stackElement(mScopeStack, compoundStatement);
	ParseNodeTraverser::Visit(compoundStatement);
}


void SemanticAnalysisPass::Visit(SwitchSection *switchSection)
{
	ScopeStack::Element stackElement(mScopeStack, switchSection);
	ParseNodeTraverser::Visit(switchSection);
}


void SemanticAnalysisPass::Visit(ForStatement *forStatement)
{
	ScopeStack::Element stackElement(mScopeStack, forStatement);
	ParseNodeTraverser::Visit(forStatement);
}


Symbol *SemanticAnalysisPass::GetSymbol(const Token *name)
{
	Symbol *parent = GetCurrentScope();
	return parent->FindSymbol(name);
}


Symbol *SemanticAnalysisPass::GetSymbol(const QualifiedIdentifier *identifier)
{
	Symbol *parent = GetCurrentScope();
	return parent->FindSymbol(identifier);
}


void SemanticAnalysisPass::InsertSymbol(Symbol *symbol)
{
	Symbol *parent = GetCurrentScope();
	InsertSymbol(parent, symbol);
}


void SemanticAnalysisPass::InsertSymbol(Symbol *parent, Symbol *symbol)
{
	Symbol *prev = parent->FindSymbol(symbol->GetName());

	if (prev != NULL)
	{
		mErrorBuffer.PushError(ParseError::DUPLICATE_SYMBOL, symbol->GetName(), prev->GetName());
	}

	parent->InsertSymbol(symbol);
}


Symbol *SemanticAnalysisPass::GetOrInsertSymbol(Symbol *symbol)
{
	Symbol *parent = GetCurrentScope();
	return GetOrInsertSymbol(parent, symbol);
}


Symbol *SemanticAnalysisPass::GetOrInsertSymbol(Symbol *parent, Symbol *symbol)
{
	Symbol *target = parent->FindSymbol(symbol->GetName());

	if ((target != NULL) && (target->GetSymbolType() != symbol->GetSymbolType()))
	{
		mErrorBuffer.PushError(ParseError::DUPLICATE_SYMBOL, target->GetName(), symbol->GetName());
		target = NULL;
	}

	if (target == NULL)
	{
		parent->InsertSymbol(symbol);
		target = symbol;
	}

	return target;
}

}