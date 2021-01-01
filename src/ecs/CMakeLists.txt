set(SOURCES entities.cc components.cc coordinator.cc)

add_library(wayward_ecs STATIC ${SOURCES})
