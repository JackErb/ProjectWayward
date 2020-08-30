set(SOURCES TextureLoader.h TextureLoader.cc Renderer.h)

add_library(ww_graphics STATIC ${SOURCES})

target_link_libraries(ww_graphics ${OPENGL_gl_LIBRARY} glad)
