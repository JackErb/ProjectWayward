target_sources(main PUBLIC GameController.hpp GameController.cpp CameraController.cpp
                 CameraController.hpp MathHelper.hpp MathHelper.cpp PlayerInput.hpp
		 PlayerInput.cpp)

add_subdirectory(Physics)
add_subdirectory(Loaders)
add_subdirectory(Entities)
add_subdirectory(Character)
