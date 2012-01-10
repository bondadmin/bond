#ifndef PRIVATE_BOND_VALUEEVALUATIONPASS_H
#define PRIVATE_BOND_VALUEEVALUATIONPASS_H

#include "private/semanticanalysispass.h"

namespace Bond
{

class TypeAndValue;

class ValueEvaluationPass: public SemanticAnalysisPass
{
public:
	ValueEvaluationPass(ParseErrorBuffer &errorBuffer, SymbolTable &symbolTable, bu32_t pointerSize):
		SemanticAnalysisPass(errorBuffer, symbolTable),
		mPrevEnumerator(NULL),
		mPointerSize(pointerSize),
		mHasResolvedItems(false),
		mHasUnresolvedItems(false)
	{}

	virtual ~ValueEvaluationPass() {}

	virtual void Analyze(TranslationUnit *translationUnitList);

protected:
	virtual void Visit(EnumDeclaration *enumDeclaration);
	virtual void Visit(Enumerator *enumerator);
	virtual void Visit(StructDeclaration *structDeclaration);
	virtual void Visit(FunctionDefinition *functionDefinition);
	virtual void Visit(Parameter *parameter);
	virtual void Visit(TypeDescriptor *typeDescriptor);
	virtual void Visit(NamedInitializer *namedInitializer);
	virtual void Visit(SwitchLabel *switchLabel);
	virtual void Visit(DeclarativeStatement *declarativeStatement);
	virtual void Visit(ConditionalExpression *conditionalExpression);
	virtual void Visit(BinaryExpression *binaryExpression);
	virtual void Visit(UnaryExpression *unaryExpression);
	virtual void Visit(PostfixExpression *postfixExpression);
	virtual void Visit(MemberExpression *memberExpression);
	virtual void Visit(ArraySubscriptExpression *arraySubscriptExpression);
	virtual void Visit(FunctionCallExpression *functionCallExpression);
	virtual void Visit(CastExpression *castExpression);
	virtual void Visit(SizeofExpression *sizeofExpression);
	virtual void Visit(ConstantExpression *constantExpression);
	virtual void Visit(IdentifierExpression *identifierExpression);

private:
	struct AlignmentComparator
	{
		bool operator()(const DeclarativeStatement &a, const DeclarativeStatement &b) const;
	};

	void Resolve(TypeAndValue &tav);
	void CheckUnresolved(const TypeAndValue &tav);

	ParseErrorBuffer mUnresolvedErrorBuffer;
	Enumerator *mPrevEnumerator;
	bu32_t mPointerSize;
	bool mHasResolvedItems;
	bool mHasUnresolvedItems;
};

}

#endif
