set(SOURCES input.cc buttons.cc)

add_library(wayward_input STATIC ${SOURCES})
target_link_libraries(wayward_input ${SDL_LIBS})
