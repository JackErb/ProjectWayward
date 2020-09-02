set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
add_library(glad util/glad.c)
target_include_directories(glad PUBLIC include/)

set(SOURCES app/main.cc app/resource_path.cc include/SDL.h include/stb_image.h util/timer.h util/timer.cc)
file(GLOB_RECURSE RES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")

if (APPLE)
    add_executable(wayward MACOSX_BUNDLE ${SOURCES} ${RES_SOURCES})
    set_source_files_properties(${RES_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION resources/)
	set(SDL_LIBS "/Library/Frameworks/SDL2.framework"
                 "/Library/Frameworks/SDL2_image.framework"
    )
endif()

add_subdirectory(math)
add_subdirectory(memory)
add_subdirectory(graphics)
add_subdirectory(game)

target_link_libraries(ww_graphics ww_memory ${SDL_LIBS} ${OPENGL_gl_LIBRARY})
target_link_libraries(ww_game ww_graphics ww_math ${SDL_LIBS})
target_link_libraries(wayward ww_game ww_math)

target_include_directories(ww_graphics PUBLIC include/)
target_include_directories(ww_game PUBLIC graphics/)
target_include_directories(wayward PUBLIC game/ graphics/ include/ util/)
