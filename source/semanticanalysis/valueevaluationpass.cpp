namespace Bond
{

class ValueEvaluationPass: public SemanticAnalysisPass
{
public:
	virtual ~ValueEvaluationPass() {}

	virtual void Analyze(TranslationUnit *translationUnitList);

protected:
	ValueEvaluationPass(ParseErrorBuffer &errorBuffer, SymbolTable &symbolTable):
		SemanticAnalysisPass(errorBuffer, symbolTable)
	{}

	//virtual void Visit(ConditionalExpression *conditionalExpression);
	//virtual void Visit(BinaryExpression *binaryExpression);
	virtual void Visit(UnaryExpression *unaryExpression);
	//virtual void Visit(PostfixExpression *postfixExpression);
	//virtual void Visit(MemberExpression *memberExpression);
	//virtual void Visit(ArraySubscriptExpression *arraySubscriptExpression);
	//virtual void Visit(FunctionCallExpression *functionCallExpression);
	//virtual void Visit(CastExpression *castExpression);
	//virtual void Visit(SizeofExpression *sizeofExpression);
	//virtual void Visit(ConstantExpression *constantExpression);
	//virtual void Visit(IdentifierExpression *identifierExpression);

private:
	bool mMadeChanges;
};


class TopLevelValueEvaluationPass: public ValueEvaluationPass
{
public:
	TopLevelValueEvaluationPass(ParseErrorBuffer &errorBuffer, SymbolTable &symbolTable):
		ValueEvaluationPass(errorBuffer, symbolTable)
	{}

	~TopLevelValueEvaluationPass() {}

protected:
	// This pass doesn't descend into structs and functions.
	virtual void Visit(StructDeclaration *structDeclaration) {}
	virtual void Visit(FunctionDefinition *functionDefinition) {}
};


void ValueEvaluationPass::Analyze(TranslationUnit *translationUnitList)
{
	/*
	do
	{
		mMadeChanges = false;
		SemanticAnalysisPass::Analyze(translationUnitList);
	}
	while (mMadeChanges);
	*/
}


void ValueEvaluationPass::Visit(UnaryExpression *unaryExpression)
{
	TypeAndValue &tav = unaryExpression->GetTypeAndValue();

	if (!tav.IsValueDefined())
	{
		ParseNodeTraverser::Visit(unaryExpression);
		const TypeAndValue &rhs = unaryExpression->GetRhs()->GetTypeAndValue();

		if (rhs.IsValueDefined())
		{
			const Token *op = unaryExpression->GetOperator();

			switch (op->GetTokenType())
			{
				case Token::OP_PLUS:
					tav.SetValue(rhs.GetValue());
					break;

				case Token::OP_MINUS:
					tav.SetValue(UnaryMinus(rhs));
					break;

				case Token::OP_NOT:
					tav.SetBoolValue(!rhs.GetBoolValue());
					break;

				case Token::OP_BIT_AND:
					// TODO
					break;

				case Token::OP_BIT_NOT:
					break;

				case Token::OP_MULT:
					// TODO
					break;

				default:
					break;
			}
		}
	}
}

}
