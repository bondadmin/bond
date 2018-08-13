#ifndef BOND_TYPES_STRINGVIEW_H
#define BOND_TYPES_STRINGVIEW_H

#include "bond/types/stringutil.h"

namespace Bond
{

class OutputStream;

// Wrapper around an externally allocated C string that handles embeded null characters
// and non-null terminated strings. Also provides some comparison operators, making it
// useful for storing in containers.
class StringView
{
public:
	StringView():
		mStr(nullptr),
		mLength(0)
	{}

	StringView(const char *str):
		mStr(str)
	{
		mLength = StringLength(str);
	}

	StringView(const char *str, size_t length):
		mStr(str),
		mLength(length)
	{}

	const char *GetString() const { return mStr; }
	size_t GetLength() const { return mLength; }
	bool IsEmpty() const { return mLength == 0; }

	void PrintTo(OutputStream &stream) const;

	bool operator==(const StringView &other) const
	{
		return StringEqual(mStr, mLength, other.mStr, other.mLength);
	}

	bool operator<(const StringView &other) const
	{
		return StringCompare(mStr, mLength, other.mStr, other.mLength) < 0;
	}

private:
	const char *mStr;
	size_t mLength;
};

}

#endif