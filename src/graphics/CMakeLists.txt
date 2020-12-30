set(SOURCES graphics.cc shaders.cc textures.cc)

add_library(wayward_graphics STATIC ${SOURCES})
target_link_libraries(wayward_graphics ${OPENGL_gl_LIBRARY} glad)
