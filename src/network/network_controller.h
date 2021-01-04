#pragma once

#include <mutex>
#include <cstdint>
#include <udp_socket.h>
#include <chrono>

class network_controller {
 public:
  int state;
  std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
  char *data;
  int data_len;
  uint32_t session_id;

  udp_socket socket;

  // Following fields are accessed by multiple threads
  std::mutex lock;
  bool terminate;
};

void run_network_controller(network_controller *);
