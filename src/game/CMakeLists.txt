set(SOURCES ../ecs/components.cc ../ecs/coordinator.cc ../ecs/entities.cc)
set(SOURCES ${SOURCES} controller.cc physics_system.cc graphics_system.cc playerinput_system.cc)

add_library(wayward_game STATIC ${SOURCES})
