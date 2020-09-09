set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
add_library(glad util/glad.c)
target_include_directories(glad PUBLIC include/)

set(SOURCES app/main.cc app/resource_path.cc include/SDL.h include/stb_image.h)
file(GLOB_RECURSE RES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")

# Optimization
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Ofast")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
endif()

if (APPLE)
    add_executable(wayward MACOSX_BUNDLE ${SOURCES} ${RES_SOURCES})
    set_source_files_properties(${RES_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION resources/)
	set(SDL_LIBS "/Library/Frameworks/SDL2.framework"
                 "/Library/Frameworks/SDL2_image.framework"
    )
endif()

add_subdirectory(math)
add_subdirectory(util)
add_subdirectory(generator)
add_subdirectory(graphics)
add_subdirectory(game)

target_link_libraries(ww_generator ww_math)
target_link_libraries(ww_graphics ${SDL_LIBS} ${OPENGL_gl_LIBRARY})
target_link_libraries(ww_game ww_graphics ww_util ww_math ww_generator ${SDL_LIBS})
target_link_libraries(wayward ww_game ww_math ww_util)

target_include_directories(ww_generator PUBLIC include/)
target_include_directories(ww_graphics PUBLIC include/)
target_include_directories(ww_game PUBLIC game/ game/entities/ game/state/ graphics/ include/ util/)
target_include_directories(wayward PUBLIC game/ math/ graphics/ include/ util/)
