file(GLOB_RECURSE RES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*")

if (APPLE)
	add_executable(main MACOSX_BUNDLE main.cpp TextureV.hpp TextureV.cpp ${RES_SOURCES})
	set_source_files_properties(${RES_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/)
else()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
	add_executable(main main.cpp TextureV.hpp TextureV.cpp)
endif()

add_subdirectory(Game)
add_subdirectory(MSL)
add_subdirectory(Network)
add_subdirectory(Generator)
add_subdirectory(MathHelper)
add_subdirectory(test)
#add_subdirectory(imgui)

target_link_libraries(main GameLib MSLLib NetworkLib GeneratorLib MathHelper)
target_link_libraries(GeneratorLib MathHelper)

if (APPLE)
	set(SDL_LIBS
		"/Library/Frameworks/SDL2.framework"
		"/Library/Frameworks/SDL2_image.framework"
	)

	set(NETWORK_LIBS
		"/Library/Frameworks/sfml-network.framework"
		"/Library/Frameworks/sfml-system.framework"
	)
else()
	set(SDL_LIBS 
		"C:/Users/Jack/Desktop/slibs/SDL2-2.0.12/lib/x64/SDL2.lib"
		"C:/Users/Jack/Desktop/slibs/SDL2-2.0.12/lib/x64/SDL2main.lib"
		"C:/Users/Jack/Desktop/slibs/SDL2_image-2.0.5/lib/x64/SDL2_image.lib"
	)

	set(NETWORK_LIBS
		"C:/Users/Jack/Desktop/slibs/SDL2-2.0.12/lib/x64/SDL2.lib"
		"C:/Users/Jack/Desktop/slibs/SDL2-2.0.12/lib/x64/SDL2main.lib"
		"C:/Users/Jack/Desktop/slibs/SDL2_image-2.0.5/lib/x64/SDL2_image.lib"
		"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/lib/sfml-network-s-d.lib"
		"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/lib/sfml-system-s-d.lib"
		"winmm.lib"
		"ws2_32.lib"
	)

	include_directories(main
		"C:/Users/Jack/Desktop/slibs/SDL2_image-2.0.5/include"
		"C:/Users/Jack/Desktop/slibs/SDL2-2.0.12/include"
		"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/include"
	)
endif()

target_link_libraries(main "${SDL_LIBS}")
target_link_libraries(GameLib GeneratorLib "${SDL_LIBS}")
target_link_libraries(NetworkLib "${NETWORK_LIBS}")


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(SFML_STATIC)

#target_include_directories(main PUBLIC
#	"${PROJECT_BINARY_DIR}"
#	"${PROJECT_SOURCE_DIR}/Game"
#	"${PROJECT_SOURCE_DIR}/Game/Character"
#	"${PROJECT_SOURCE_DIR}/Game/Character/CharacterState"
#	"${PROJECT_SOURCE_DIR}/Game/Character/CharacterState/Airborne"
#	"${PROJECT_SOURCE_DIR}/Game/Character/CharacterState/Grounded"
#	"${PROJECT_SOURCE_DIR}/Game/Entities/"
#	"${PROJECT_SOURCE_DIR}/Game/Loaders"
#	"${PROJECT_SOURCE_DIR}/Game/Physics"
#	"${PROJECT_SOURCE_DIR}/MSL"
#	"${PROJECT_SOURCE_DIR}/MSL/AST"
#	"${PROJECT_SOURCE_DIR}/MSL/AST/Expressions"
#	"${PROJECT_SOURCE_DIR}/MSL/AST/Statements"
#	"${PROJECT_SOURCE_DIR}/Network"
#)

install(TARGETS main DESTINATION bin)
