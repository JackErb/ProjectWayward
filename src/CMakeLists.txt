set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
add_library(glad util/glad.c)
target_include_directories(glad PUBLIC include/)

set(SOURCES app/main.cc include/SDL.h util/timer.h util/timer.cc)
file(GLOB_RECURSE RES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")

if (APPLE)
    add_executable(wayward MACOSX_BUNDLE ${SOURCES})
    set_source_files_properties(${RES_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION resources/)
	set(SDL_LIBS "/Library/Frameworks/SDL2.framework"
                 "/Library/Frameworks/SDL2_image.framework"
    )
endif()

add_subdirectory(memory)
add_subdirectory(graphics)
add_subdirectory(game)

target_link_libraries(ww_graphics ww_memory ${SDL_LIBS} ${OPENGL_gl_LIBRARY})
target_link_libraries(ww_game ww_graphics ${SDL_LIBS})
target_link_libraries(wayward ww_game)

target_include_directories(ww_graphics PUBLIC include/)
target_include_directories(wayward PUBLIC game/ include/ util/)
