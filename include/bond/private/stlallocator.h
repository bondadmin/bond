#ifndef PRIVATE_BOND_STLALLOCATOR_H
#define PRIVATE_BOND_STLALLOCATOR_H

#include "bond/allocator.h"

namespace Bond
{

template <typename T>
class StlAllocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef T &reference;
	typedef const T &const_reference;
	typedef T value_type;
	template <class U> struct rebind { typedef StlAllocator<U> other; };

	StlAllocator():
		mAllocator(NULL)
	{}

	StlAllocator(Allocator *allocator):
		mAllocator(allocator)
	{}

	StlAllocator(const StlAllocator<T> &other):
		mAllocator(other.GetAllocator())
	{}

	template <class U> StlAllocator(const StlAllocator<U> &other):
		mAllocator(other.GetAllocator())
	{}

  ~StlAllocator() {}

	pointer address(reference t) const { return &t; }

	const_pointer address(const_reference t) const { return &t; }

	pointer allocate(size_type numElements, const void* = NULL)
	{
		return mAllocator->Alloc<T>(numElements);
	}

	void deallocate(pointer p, size_type)
	{
		mAllocator->Free(p);
	}

	void construct(pointer p, const_reference t)
	{
		new ((void *) p) T(t);
	}

	void destroy(pointer p)
	{
		p->~T();
	}

	size_type max_size() const { return size_type(-1) / sizeof(T); }

	Allocator *GetAllocator() const { return mAllocator; }

private:
	Allocator *mAllocator;
};


template <typename T1, typename T2>
bool operator ==(const StlAllocator<T1> &a, const StlAllocator<T2> &b)
{
	return a.GetAllocator() == b.GetAllocator();
}


template <typename T1, typename T2>
bool operator !=(const StlAllocator<T1> &a, const StlAllocator<T2> &b)
{
	return a.GetAllocator() != b.GetAllocator();
}

}

#endif
