set(SOURCES TextureLoader.cc SpriteBuffer.h SpriteBuffer.cc
            WaywardGL.h WaywardGL.cc ShaderLoader.h ShaderLoader.cc ResourceManager.h
            ResourceManager.cc ShapeBuffer.h ShapeBuffer.cc MetaballBuffer.h
            MetaballBuffer.cc)

add_library(ww_graphics STATIC ${SOURCES})

target_link_libraries(ww_graphics ${OPENGL_gl_LIBRARY} glad)
