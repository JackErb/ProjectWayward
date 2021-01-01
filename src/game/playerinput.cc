#include <ecs_defs.h>
#include <ecs.h>
#include <coordinator.h>
#include <logs.h>
#include <ww_math.h>
#include <fp_overloads.h>
#include <input.h>

// PlayerInput
void PlayerInputSystem::RegisterPlayer(Entity player) {
  entities.insert(player);

  // Init input component
  Input *input = coordinator->GetComponent<Input>(player);
  input->input = input::GetInput(0);
}

void PlayerInputSystem::Update() {
  for (Entity e : entities) {
    Input *input_component = coordinator->GetComponent<Input>(e);
    Transform *transform = coordinator->GetComponent<Transform>(e);

    const struct input::player_input *input = input_component->input;
    transform->position.x += fp::from_int(input->stick_x * 100);
    transform->position.y += fp::from_int(input->stick_y * 100);
  }
}
