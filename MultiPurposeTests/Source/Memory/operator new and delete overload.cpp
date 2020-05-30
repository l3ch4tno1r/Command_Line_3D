#include <iostream>

static uint32_t numallocation   = 0;
static uint32_t numdeallocation = 0;

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl
#define DISPLAY_ALLOCATIONS

#ifdef DISPLAY_ALLOCATIONS
#define LOG(X) X
#else
#define LOG(X)
#endif // DISPLAY_ALLOCATIONS


void* operator new(size_t size)
{
	++numallocation;

	LOG(std::cout << "Allocated    : " << size << " bytes." << std::endl);

	return malloc(size);
}

void* operator new[](size_t size)
{
	++numallocation;

	LOG(std::cout << "Allocated    : " << size << " bytes." << std::endl);

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	--numdeallocation;

	LOG(std::cout << "Deallocating : " << size << " bytes." << std::endl);

	free(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	--numdeallocation;

	LOG(std::cout << "Deallocating : " << size << " bytes." << std::endl);

	free(ptr);
}