#ifndef BOND_PRIVATE_VECTOR_H
#define BOND_PRIVATE_VECTOR_H

#include "bond/stl/stlallocator.h"
#include <vector>

namespace Bond
{

template <typename ValueType>
using Vector = std::vector<ValueType, StlAllocator<ValueType> >;

}

#endif
