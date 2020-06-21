file(GLOB SOURCES "*.h" "*.cpp")

message(${SOURCES})

add_library(imgui ${SOURCES})

target_link_libraries(imgui 
	"/Library/Frameworks/SDL2.framework"
)

#add_definitions("-I./libs/gl3w -DIMGUI_IMPL_OPEN_GL_LOADER_GL3W")

#target_link_libraries(imgui "-framework Cocoa -framework OpenGL")
