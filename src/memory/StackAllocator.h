#ifndef StackAllocator_h
#define StackAllocator_h

#include <iostream>

class StackAllocator {
  public:
    StackAllocator(int size);
    ~StackAllocator();

    template <class T>
    void* raw_allocate() {
        int size = sizeof(T);
        if ((free_chunk + size) - pool_head >= pool_size) {
            std::cerr << "FATAL ERROR: Ran out of memory in StackAllocator" << std::endl;
            exit(1);
        }

        char* ptr = free_chunk;
        free_chunk += size;
        return reinterpret_cast<void*>(ptr);
    }

    template <class T>
    T* allocate() {
        void *ptr = raw_allocate<T>();
        return new(ptr) T();
    }

  private:
    int pool_size;
    char* pool_head;
    char* free_chunk;
};

#endif  /* StackAllocator_h */
