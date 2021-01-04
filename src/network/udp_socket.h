#pragma once

#include <cstdint>

class udp_socket {
 public:
  bool bind(unsigned short port);
  void send(const char *data, int len);
  int receive(char *data, int max_len);

  void set_send_addr(uint32_t addr, uint16_t port);

 private:
  int sockfd;
  uint32_t send_addr;
  uint16_t send_port;
};
