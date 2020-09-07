#ifndef PlayerInput_h
#define PlayerInput_h

#include <ww_math.h>
#include <vector>
#include <unordered_map>

enum ButtonState {
    Pressed, Released, Held
};

enum ButtonAction {
    Button_Jump, Button_Attack, Button_Other
};

static const ButtonAction Actions[] = {Button_Jump, Button_Attack, Button_Other};

typedef struct _SDL_GameController SDL_GameController;

struct StickState {
    FixedPoint x;
    FixedPoint y;
    FixedPoint angle;
    FixedPoint hyp;

    enum StickDir {
        Down, Left, Right, Up
    };
    bool inDir(StickDir dir) const;

    static const FixedPoint DEADZONE;
};

struct PlayerInput {
    void updateButtonsState();
    void tick();

    bool isPressed(ButtonAction action, bool orHeld=true) const;
    bool isReleased(ButtonAction action) const;

    int frame = 0;
    int gc_index;

    StickState stick;
    StickState cstick;

    std::unordered_map<ButtonAction, ButtonState> buttons;
    SDL_GameController *gc = NULL;
};

#endif  /* PlayerInput_h */
