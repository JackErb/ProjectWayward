#ifndef StackAllocator_h
#define StackAllocator_h

class StackAllocator {
  public:
	StackAllocator(int size);
	~StackAllocator();

	void* allocate(int size);

  private:
    int pool_size;
    char* pool_head;
	char* free_chunk;
};

#endif  /* StackAllocator_h */
