add_library(GameLib STATIC GameController.hpp GameController.cpp CameraController.cpp
	CameraController.hpp PlayerInput.hpp PlayerInput.cpp RogueController.hpp RogueController.cpp
	AbstractGameController.hpp
)

add_subdirectory(Physics)
add_subdirectory(Loaders)
add_subdirectory(Entities)
add_subdirectory(Character)
