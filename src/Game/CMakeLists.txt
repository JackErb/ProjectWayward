target_sources(main PUBLIC GameController.hpp GameController.cpp
                 CameraController.hpp MathHelper.hpp MathHelper.cpp PlayerInput.hpp)

add_subdirectory(Physics)
add_subdirectory(Loaders)
add_subdirectory(Entities)
add_subdirectory(Character)