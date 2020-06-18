cmake_minimum_required(VERSION 3.1)
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()
cmake_policy(SET CMP0076 NEW)

project(Wayward)

add_subdirectory(src)
