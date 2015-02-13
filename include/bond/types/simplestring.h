#ifndef BOND_TYPES_SIMPLESTRING_H
#define BOND_TYPES_SIMPLESTRING_H

#include "bond/types/stringutil.h"

namespace Bond
{

// Wrapper around an externally allocated C string that handles embeded null characters
// and non-null terminated strings. Also provides some comparison operators, making it
// useful for storing in containers.
class SimpleString
{
public:
	SimpleString():
		mStr(nullptr),
		mLength(0)
	{}

	SimpleString(const char *str):
		mStr(str)
	{
		mLength = StringLength(str);
	}

	SimpleString(const char *str, size_t length):
		mStr(str),
		mLength(length)
	{}

	const char *GetString() const { return mStr; }
	size_t GetLength() const { return mLength; }
	bool IsEmpty() const { return mLength == 0; }

	bool operator==(const SimpleString &other) const
	{
		return StringEqual(mStr, mLength, other.mStr, other.mLength);
	}

	bool operator<(const SimpleString &other) const
	{
		return StringCompare(mStr, mLength, other.mStr, other.mLength) < 0;
	}

private:
	const char *mStr;
	size_t mLength;
};

}

#endif
