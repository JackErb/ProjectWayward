#pragma once

#include <rbnetwork.h>
#include <input.h>
#include <mutex>

struct buffered_input {
  int frame;
  bool verified;
  struct input::player_input input;
};

struct input_buffer {
  std::mutex lock;
  int last_verified_index;
  int index;
  struct buffered_input inputs[rb::MaxRollbackFrames][rb::NumPlayers];

  // Returns true if last_verified_index is updated
  bool insert_input(int player, int frame);
  void get_input(int player, int frame);
};
