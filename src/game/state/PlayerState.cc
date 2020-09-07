#include "PlayerState.h"
#include "Player.h"

void PlayerState::switchState(PlayerState *new_state) {
    player->switchState(new_state);   
}
