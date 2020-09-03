set(SOURCES TextureLoader.cc Renderer.h Renderer.cc
            WaywardGL.h WaywardGL.cc Shader.h Shader.cc)

add_library(ww_graphics STATIC ${SOURCES})

target_link_libraries(ww_graphics ${OPENGL_gl_LIBRARY} glad)
