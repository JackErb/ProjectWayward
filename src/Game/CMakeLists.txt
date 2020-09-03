set(SOURCES GameController.h GameController.cc entities/Entity.h entities/Player.h entities/Player.cc
			Input.h Input.cc state/PlayerState.h state/GroundedState.h state/GroundedState.cc
			PhysicsEngine.h PhysicsEngine.cc entities/Chunk.h entities/Chunk.cc)

add_library(ww_game STATIC ${SOURCES})
