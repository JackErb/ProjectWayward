add_executable(main main.cpp)

add_subdirectory(Game)
add_subdirectory(MSL)
add_subdirectory(Network)

target_link_libraries(main
	"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/lib/sfml-graphics-s-d.lib"
	"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/lib/sfml-network-s-d.lib"
	"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/lib/sfml-window-s-d.lib"
	"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/lib/sfml-system-s-d.lib"
	"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/lib/freetype.lib"
	"opengl32.lib"
	"gdi32.lib"
	"winmm.lib"
	"ws2_32.lib")

install(TARGETS main DESTINATION bin)

add_compile_definitions(SFML_STATIC)

target_include_directories(main PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/Game"
	"${PROJECT_SOURCE_DIR}/Game/Character"
	"${PROJECT_SOURCE_DIR}/Game/Character/CharacterState"
	"${PROJECT_SOURCE_DIR}/Game/Character/CharacterState/Airborne"
	"${PROJECT_SOURCE_DIR}/Game/Character/CharacterState/Grounded"
	"${PROJECT_SOURCE_DIR}/Game/Entities/"
	"${PROJECT_SOURCE_DIR}/Game/Loaders"
	"${PROJECT_SOURCE_DIR}/Game/Physics"
	"${PROJECT_SOURCE_DIR}/MSL"
	"${PROJECT_SOURCE_DIR}/MSL/AST"
	"${PROJECT_SOURCE_DIR}/MSL/AST/Expressions"
	"${PROJECT_SOURCE_DIR}/MSL/AST/Statements"
	"${PROJECT_SOURCE_DIR}/Network"
	"${PROJECT_SOURCE_DIR}/Resources"
	"C:/Users/Jack/Desktop/slibs/SFML-2.5.1/include")