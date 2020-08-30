set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES app/main.cc include/SDL.h util/timer.h util/timer.cc)

if (APPLE)
    add_executable(wayward MACOSX_BUNDLE ${SOURCES})
    set(SDL_LIBS "/Library/Frameworks/SDL2.framework"
                 "/Library/Frameworks/SDL2_image.framework"
    )
endif()

add_subdirectory(graphics)
add_subdirectory(game)

target_link_libraries(ww_game ww_graphics)
target_link_libraries(wayward ww_game ww_graphics)

target_include_directories(wayward PUBLIC game/ include/ util/)
target_link_libraries(wayward ${SDL_LIBS})
