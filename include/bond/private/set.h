#ifndef BOND_PRIVATE_SET_H
#define BOND_PRIVATE_SET_H

#include "bond/stl/stlallocator.h"
#include <set>

namespace Bond
{

template <typename ValueType, typename CompareType = std::less<ValueType> >
struct Set
{
	typedef CompareType Compare;
	typedef StlAllocator<ValueType> Allocator;
	typedef std::set<ValueType, CompareType, Allocator> Type;
	typedef std::pair<typename Type::iterator, bool> InsertResult;
};

}

#endif
