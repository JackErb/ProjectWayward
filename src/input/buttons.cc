#include <input.h>
#include <logs.h>

bool is_kb_button_pressed(const struct input::input_device *dev, int button) {
  int key;
  switch (button) {
   case input::ButtonA:
    key = SDL_SCANCODE_Z;
    break;
   case input::ButtonB:
    key = SDL_SCANCODE_X;
    break;
   case input::ButtonC:
    key = SDL_SCANCODE_C;
    break;
   case input::ButtonD:
    key = SDL_SCANCODE_V;
    break;
  }
  return dev->keys[key];
}

bool is_gc_button_pressed(const struct input::input_device *dev, int button) {
  return false;
}

bool input::IsButtonPressed(const struct input::input_device *dev, int button) {
  switch (dev->device_type) {
   case input::DevKeyboard:
    return is_kb_button_pressed(dev, button);
   case input::DevJoystick:
    return is_gc_button_pressed(dev, button);
  }
  fatalerror("IsButtonPressed() : invalid input device type");
  return false;
}
