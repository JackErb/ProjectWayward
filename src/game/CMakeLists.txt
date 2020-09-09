set(SOURCES GameController.h GameController.cc Input.h Input.cc
            entities/Entity.h entities/Entity.cc entities/Player.h entities/Player.cc entities/Chunk.h entities/Chunk.cc
            entities/Explosive.h entities/Explosive.cc entities/PhysicsBitmask.h
            state/PlayerState.h state/PlayerState.cc state/GroundedState.h state/GroundedState.cc
            state/AirborneState.h state/AirborneState.cc
			PhysicsEngine.h PhysicsEngine.cc PhysicsMultithreader.h PhysicsMultithreader.cc)

add_library(ww_game STATIC ${SOURCES})
