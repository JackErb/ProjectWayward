#include <udp_socket.h>
#include <logs.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>

bool udp_socket::bind(unsigned short port) {
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    cprint("socket creation failed.");
    return false;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);
  if (::bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
    cprint("socket bind failed.");
    return false;
  }

  cprint("socket succesfully binded");
  return true;
}

void udp_socket::send(const char *data, int len) {
  // Making the address here is temporary
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = send_port;
  addr.sin_addr.s_addr = send_addr;

  int flags = 0;
  int n = sendto(sockfd, data, len, flags, (const struct sockaddr*)&addr, sizeof(addr));
  if (n == -1) {
    cprint("sendto() returned error");
  } else {
    cprint("Message sent.");
  }
}

int udp_socket::receive(char *data, int max_len) {
  // Making the address here is temporary
  /*struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = 24240;
  addr.sin_addr.s_addr = INADDR_ANY;*/

  int flags = MSG_DONTWAIT;
  int n = recvfrom(sockfd, data, max_len, flags, NULL, 0);

  if (n < 0) {
    if (errno != EWOULDBLOCK || errno != EAGAIN) {
      cprint("recvfrom() returned error");
    }
    return 0;
  } else {
    cprint("Message received");
    return n;
  }
}

void udp_socket::set_send_addr(uint32_t addr, uint16_t port) {
  send_addr = htonl(addr);
  send_port = htons(port);
}
