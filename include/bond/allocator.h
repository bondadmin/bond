#ifndef BOND_ALLOCATOR_H
#define BOND_ALLOCATOR_H

namespace Bond
{

class Allocator
{
public:
	virtual ~Allocator() {}
	virtual void *Alloc(int size) = 0;
	virtual void Free(void *buffer) = 0;

	template <typename T> T *Alloc() { return static_cast<T *>(Alloc(sizeof(T))); }
	template <typename T> T *Alloc(int numElements) { return static_cast<T *>(Alloc(sizeof(T) * numElements)); }
};

}

#endif
