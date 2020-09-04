set(SOURCES TextureLoader.cc SpriteBuffer.h SpriteBuffer.cc
            WaywardGL.h WaywardGL.cc Shader.h Shader.cc ResourceManager.h
            ResourceManager.cc)

add_library(ww_graphics STATIC ${SOURCES})

target_link_libraries(ww_graphics ${OPENGL_gl_LIBRARY} glad)
