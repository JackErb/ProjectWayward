set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.9)

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0076 NEW)

project(Wayward)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
add_library(glad ../libs/glad/glad.c)
target_include_directories(glad PUBLIC ../libs/glad)

set(WAYWARD_SOURCES main.cc util/logs.cc util/resources.cc util/ww_math.cc)
file(GLOB_RECURSE RES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../resources/*")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
endif()

if (APPLE)
    add_executable(wayward MACOSX_BUNDLE ${WAYWARD_SOURCES} ${RES_SOURCES})
    set_source_files_properties(${RES_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION resources/)
	set(SDL_LIBS "/Library/Frameworks/SDL2.framework"
                 "/Library/Frameworks/SDL2_image.framework"
    )
endif()

# Optimization
set(OPTIMIZATION_FLAG "-O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OPTIMIZATION_FLAG}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${OPTIMIZATION_FLAG}")
set_property(TARGET wayward PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

add_subdirectory(graphics)
add_subdirectory(input)
add_subdirectory(physics)
add_subdirectory(network)
add_subdirectory(game)

target_include_directories(wayward_graphics PRIVATE ../inc/)
target_include_directories(wayward_input PRIVATE ../inc/)
target_include_directories(wayward_network PRIVATE ../inc/)
target_include_directories(wayward_game PRIVATE ../inc/)
target_include_directories(wayward_physics PRIVATE ../inc/)
target_include_directories(wayward PRIVATE ../inc/)
target_link_libraries(wayward ${OPENGL_gl_LIBRARY} ${SDL_LIBS} glad
                              wayward_graphics wayward_input wayward_game wayward_physics
                              wayward_network)
