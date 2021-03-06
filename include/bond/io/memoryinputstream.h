#ifndef BOND_IO_MEMORYINPUTSTREAM_H
#define BOND_IO_MEMORYINPUTSTREAM_H

#include "bond/io/inputstream.h"
#include "bond/types/types.h"

namespace Bond
{

/// \brief A concrete implementation of InputStream that performs input operations from an
/// in-memory buffer.
/// \ingroup io
class MemoryInputStream: public InputStream
{
public:
	MemoryInputStream(const void *buffer, pos_t size);
	virtual ~MemoryInputStream() {}

	virtual void VScan(const char *format, va_list argList) override;
	virtual size_t Read(uint8_t *bytes, size_t numBytes) override;
	virtual int Read() override;

	virtual pos_t GetPosition() const override { return mCurrent; }
	virtual pos_t GetEndPosition() const override { return mSize; }
	virtual void SetPosition(off_t offset) override;
	virtual void SetPositionFromEnd(off_t offset) override;
	virtual void AddOffset(off_t offset) override;

	virtual bool IsEof() const override { return mCurrent >= mSize; }
	virtual bool HasError() const override { return false; }
	virtual void ClearError() override {}

private:
	const uint8_t *mBuffer;
	pos_t mSize;
	pos_t mCurrent;
};

}

#endif
