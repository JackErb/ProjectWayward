set(SOURCES controller.cc)

add_library(wayward_game STATIC ${SOURCES})
target_link_libraries(wayward_game wayward_ecs)
