#ifndef BOND_AUTOSTACK_H
#define BOND_AUTOSTACK_H

namespace Bond
{

template<typename ElementType>
class AutoStack
{
public:
	class Element
	{
	public:
		explicit Element(AutoStack &stack):
			mStack(stack),
			mNext(0)
		{
			mStack.Push(this);
		}

		Element(AutoStack &stack, const ElementType &value):
			mValue(value),
			mStack(stack),
			mNext(0)
		{
			mStack.Push(this);
		}

		Element(AutoStack &stack, const Element &other):
			mValue(other.mValue),
			mStack(stack),
			mNext(0)
		{
			mStack.Push(this);
		}

		Element(const Element &other):
			mValue(other.mValue),
			mStack(other.mStack),
			mNext(0)
		{
			mStack.Push(this);
		}

		~Element()
		{
			mStack.Pop();
		}

		Element &operator=(const Element &other) { mValue = other.mValue; return *this; }
		Element &operator=(const ElementType &value) { mValue = value; return *this; }

		operator ElementType&() { return mValue; }
		operator const ElementType&() const { return mValue; }

		ElementType *operator->() { return &mValue; }
		const ElementType *operator->() const { return &mValue; }

		ElementType &GetValue() { return mValue; }
		const ElementType &GetValue() const  { return mValue; }
		void SetValue(const ElementType &value) { mValue = value; }

	private:
		friend class AutoStack;

		Element *GetNext() { return mNext; }
		void SetNext(Element *next) { mNext = next; }

		ElementType mValue;
		AutoStack &mStack;
		Element *mNext;
	};

	AutoStack(): mTop(0) {}

	void Push(Element *element)
	{
		element->SetNext(mTop);
		mTop = element;
	}

	void Pop()
	{
		if (mTop != 0)
		{
			mTop = mTop->GetNext();
		}
	}

	Element &GetTop() { return *mTop; }

private:

	Element *mTop;
};

}

#endif