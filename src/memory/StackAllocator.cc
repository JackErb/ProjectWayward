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
