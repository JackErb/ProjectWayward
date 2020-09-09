set(SOURCES timer.h timer.cc StackAllocator.h StackAllocator.cc glad.c)

add_library(ww_util STATIC ${SOURCES})
target_include_directories(ww_util PUBLIC ../include)
