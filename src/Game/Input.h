#ifndef PlayerInput_h
#define PlayerInput_h

#include <ww_math.h>
#include <vector>
#include <unordered_map>

enum ButtonState {
    Pressed, Released, Held
};

enum ButtonAction {
    Jump, Attack
};

typedef struct _SDL_GameController SDL_GameController;

struct StickState {
    FixedPoint x;
    FixedPoint y;
    FixedPoint angle;
    FixedPoint hyp;
};

struct PlayerInput {
    void updateButtonsState();
    void tick();

    int frame = 0;
    int gc_index;

    StickState stick;
    StickState cstick;

    std::unordered_map<ButtonAction, ButtonState> buttons;
    SDL_GameController *gc = NULL;
};

#endif  /* PlayerInput_h */
