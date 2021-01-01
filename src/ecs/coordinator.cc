#include <coordinator.h>
#include <logs.h>

GameStateCoordinator::GameStateCoordinator() {

}

Entity GameStateCoordinator::CreateEntity() {
  return entity_manager_.CreateEntity();
}
