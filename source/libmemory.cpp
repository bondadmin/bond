#include "bond/api/libmemory.h"
#include "bond/systems/allocator.h"
#include "bond/vm/vm.h"
#include <cstring>

namespace Bond
{

void Allocate(Bond::CalleeStackFrame &frame)
{
	Allocator &allocator = frame.GetVM().GetAllocator();
	const size_t size = size_t(frame.GetArg<uint64_t>(0));
	void *ptr = allocator.Allocate(size);
	frame.SetReturnValue(ptr);
}


void AllocateAligned(Bond::CalleeStackFrame &frame)
{
	Allocator &allocator = frame.GetVM().GetAllocator();
	const size_t size = size_t(frame.GetArg<uint64_t>(0));
	const size_t alignment = size_t(frame.GetArg<uint32_t>(1));
	void *ptr = allocator.AllocateAligned(size, alignment);
	frame.SetReturnValue(ptr);
}


void Free(Bond::CalleeStackFrame &frame)
{
	Allocator &allocator = frame.GetVM().GetAllocator();
	void *ptr = frame.GetArg<void *>(0);
	allocator.Free(ptr);
}


void FreeAligned(Bond::CalleeStackFrame &frame)
{
	Allocator &allocator = frame.GetVM().GetAllocator();
	void *ptr = frame.GetArg<void *>(0);
	allocator.FreeAligned(ptr);
}


void Memcpy(Bond::CalleeStackFrame &frame)
{
	void *dest = frame.GetArg<void *>(0);
	void *src = frame.GetArg<void *>(1);
	size_t size = size_t(frame.GetArg<uint64_t>(2));
	memcpy(dest, src, size);
}

}
