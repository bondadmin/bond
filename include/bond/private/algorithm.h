#ifndef PRIVATE_BOND_ALGORITHM_H
#define PRIVATE_BOND_ALGORITHM_H

#include <algorithm>

namespace Bond
{

template <typename RandomAccessIterator, typename Comparator>
void Sort(RandomAccessIterator first, RandomAccessIterator last, Comparator comparator)
{
	std::sort(first, last, comparator);
}


template <typename ForwardIterator, typename ValueType>
ForwardIterator LowerBound(ForwardIterator first, ForwardIterator last, const ValueType &value)
{
	return std::lower_bound(first, last, value);
}

}

#endif