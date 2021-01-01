set(SOURCES ../ecs/components.cc ../ecs/coordinator.cc ../ecs/entities.cc)
set(SOURCES ${SOURCES} controller.cc physics.cc graphics.cc playerinput.cc)

add_library(wayward_game STATIC ${SOURCES})
target_link_libraries(wayward_game)
