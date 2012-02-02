#ifndef BOND_BINARYWRITER_H
#define BOND_BINARYWRITER_H

namespace Bond
{

class BinaryWriter
{
public:
	virtual ~BinaryWriter() {}
	virtual void Write(unsigned char c) = 0;
	virtual int GetPosition() const = 0;
	virtual void SetPosition(int offset) = 0;
	virtual void SetPositionFromEnd(int offset) = 0;
	virtual void AddOffset(int offset) = 0;
};

}

#endif
