#pragma once

#include <cstdint>

typedef struct _SDL_GameController SDL_GameController;
typedef uint8_t Uint8;

namespace input {

static const int DevKeyboard = 0;
static const int DevJoystick = 1;

static const int NumButtons = 4;
static const int ButtonA = 0;
static const int ButtonB = 1;
static const int ButtonC = 2;
static const int ButtonD = 3;

static const int ButtonPressed  = 0;
static const int ButtonHeld     = 1;
static const int ButtonReleased = 2;

struct player_input {
  int stick_x; // Stick values on scale of 0 to 100
  int stick_y;
  int buttons[NumButtons];
};

struct input_device {
  player_input input;
  bool in_use;
  bool connected;
  int device_type;

  union {
    SDL_GameController *gc; // Used for game controller
    const Uint8 *keys;      // Used for keyboard
  };
};

void Init();
void Update();

const struct player_input *GetInput(int);

/* Used internally by input lib */
bool IsButtonPressed(const struct input_device *, int);

};
