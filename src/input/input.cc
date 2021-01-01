#include <input.h>
#include <logs.h>

#include <SDL.h>

#if defined(__APPLE__)
#include <SDL2/SDL_gamecontroller.h>
#endif

using input::input_device;

/* STATIC DATA */
struct input_device player_device;

void update_sticks(struct input_device *);
void update_buttons(struct input_device *);

void connect_input_device(struct input_device *device) {
  switch (device->device_type) {
   case input::DevKeyboard:
    device->keys = SDL_GetKeyboardState(NULL);
    device->connected = device->keys != NULL;
    break;
   case input::DevJoystick:
    device->gc = SDL_GameControllerOpen(0);
    device->connected = device->gc != NULL;
    break;
  }
}


void input::Init() {
  player_device.in_use = true;
  player_device.connected = false;
  player_device.device_type = input::DevKeyboard;
}

void input::Update() {
  if (!player_device.connected) {
    connect_input_device(&player_device);
    return;
  }

  update_sticks(&player_device);
  update_buttons(&player_device);
}

const struct input::player_input *input::GetInput(int player) {
  return &player_device.input;
}

void update_sticks(struct input_device *device) {
  switch (device->device_type) {
   case input::DevKeyboard: {
    const Uint8 *keys = device->keys;
    device->input.stick_x = keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A];
    device->input.stick_y = keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W];
    break;
   }
   case input::DevJoystick: {
    break;
   }
  }
}

void update_buttons(struct input_device *device) {
  // Shift button state of Pressed and Held
  for (int i = 0; i < input::NumButtons; i++) {
    bool was_released = device->input.buttons[i] == input::ButtonReleased;
    bool is_pressed = IsButtonPressed(device, i);

    if (was_released && is_pressed) {
      device->input.buttons[i] = input::ButtonPressed;
    } else if (!was_released && !is_pressed) {
      device->input.buttons[i] = input::ButtonReleased;
    } else {
      device->input.buttons[i] = input::ButtonHeld;
    }
  }
}
