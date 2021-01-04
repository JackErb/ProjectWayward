#pragma once

#include <input.h>
#include <vector>

namespace rb {
  static const int MaxRollbackFrames = 10;
  static const int NumPlayers = 2;

  // Sets up network controller
  void Init();
  void Shutdown();

  void StartGame();
  void Update();

  // Sets player input for the current frame
  void VerifyPlayerInput(struct input::player_input);

  // Gets all player inputs for given frame
  // Asserts false if frame is not in current input buffer bounds
  std::vector<struct input::player_input> GetInput(int);

  struct rollback {
    bool should_rollback;
    int frame;
  };
  struct rollback CheckForRollback();
}
