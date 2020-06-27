add_library(imgui imconfig.h imgui.cpp imgui.h imgui_demo.cpp imgui_draw.cpp imgui_impl_sdl.cpp
		imgui_impl_sdl.h imgui_internal.h imgui_sdl.cpp imgui_sdl.h imgui_widgets.cpp
		imstb_rectpack.h imstb_textedit.h imstb_truetype.h)

target_link_libraries(imgui 
	"C:/Users/Jack/Desktop/slibs/SDL2-2.0.12/lib/x64/SDL2.lib"
)

include_directories(imgui
	"C:/Users/Jack/Desktop/slibs/SDL2-2.0.12/include"
)