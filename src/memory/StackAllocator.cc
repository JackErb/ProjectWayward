#include "StackAllocator.h"

#include <iostream>

using std::cerr;
using std::endl;

StackAllocator::StackAllocator(int size) {
	pool_size = size;	
 	pool_head = new char[size];
	free_chunk = pool_head;
}

StackAllocator::~StackAllocator() {
	delete[] pool_head;
}

void* StackAllocator::allocate(int size) {
	if ((free_chunk + size) - pool_head >= pool_size) {
		cerr << "FATAL ERROR: Ran out of memory in StackAllocator" << endl;
		exit(1);
	}

	char* ptr = free_chunk;
	free_chunk += size;
	return ptr;
}
