#include <ecs_defs.h>
#include <ecs.h>
#include <coordinator.h>
#include <logs.h>
#include <ww_math.h>
#include <fp_overloads.h>
#include <input.h>

const fixed_point GroundFriction = fp::from_float(0.88f);
const fixed_point AirFriction    = fp::from_float(0.91f);
const fixed_point Gravity        = fp::from_float(10.5f);

const fixed_point ShortJump  = fp::from_float(190.f);
const fixed_point FullJump   = fp::from_float(250.f);
const fixed_point DoubleJump = fp::from_float(230.f);

const fixed_point DashSpeed = fp::from_float(140.f);
const fixed_point AirAccel    = fp::from_float(12.f);
const fixed_point MaxAirSpeed = fp::from_float(130.f);
const fixed_point MaxFall     = fp::from_float(-250.f);
const fixed_point MaxFastFall = fp::from_float(-350.f);


// PlayerInput
void PlayerInputSystem::RegisterPlayer(Entity player) {
  entities.insert(player);

  // Init input component
  Input *input = coordinator->GetComponent<Input>(player);
  input->input = input::GetInput(0);
}

void PlayerInputSystem::Update() {
  for (Entity e : entities) {
    UpdatePlayer(e);
  }
}

void PlayerInputSystem::UpdatePlayer(Entity player) {
  Input *input_component = coordinator->GetComponent<Input>(player);
  Transform *transform = coordinator->GetComponent<Transform>(player);

  const struct input::player_input *input = input_component->input;
  if (transform->grounded) {
    transform->velocity.x = fp::from_int(input->stick_x) * DashSpeed;
    transform->velocity.x *= GroundFriction;
  } else {
    transform->velocity.x += fp::from_int(input->stick_x) * AirAccel;
    transform->velocity.x *= AirFriction;

    fixed_point vx = transform->velocity.x;
    if (fp::abs(vx) > MaxAirSpeed) {
      transform->velocity.x = fp::sign(vx) * MaxAirSpeed;
    }
  }

 if (input->buttons[input::ButtonA] == input::ButtonPressed) {
   cprint("Jump!");
   transform->velocity.y = DoubleJump;
 }

  transform->velocity.y -= Gravity;
  if (transform->velocity.y < MaxFall) {
    transform->velocity.y = MaxFall;
  }
}
