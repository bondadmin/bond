#ifndef BOND_TOOLS_CBOUTIL_H
#define BOND_TOOLS_CBOUTIL_H

#include "bond/types/types.h"

namespace Bond
{

#define BOND_LIST_BLOB_ID "List"
#define BOND_FUNCTION_BLOB_ID "Func"
#define BOND_DATA_BLOB_ID "Data"
const size_t BOND_BLOB_ID_LENGTH = 4;

inline uint32_t EncodeSizeAndType(uint32_t size, SignatureType type)
{
	return (size << 4) | (type & 0xf);
}


inline void DecodeSizeAndType(uint32_t sizeAndType, uint32_t &size, SignatureType &type)
{
	size = sizeAndType >> 4;
	type = SignatureType(sizeAndType & 0xf);
}


inline PointerSize DecodePointerSize(uint16_t flags)
{
	return ((flags & 1) != 0) ? POINTER_64BIT : POINTER_32BIT;
}


inline uint16_t EncodePointerSize(uint16_t flags, PointerSize pointerSize)
{
	return flags | ((pointerSize == POINTER_64BIT) ? 1 : 0);
}

}

#endif
