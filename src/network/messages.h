#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <logs.h>

enum message_type {
  mtConnect, mtAckConnect,
  mtInput
};


struct msg_header {
  uint32_t session_id;
  uint16_t msg_type;

  void to_network() {
    session_id = htonl(session_id);
    msg_type = htons(msg_type);
  }
  void to_host() {
    session_id = ntohl(session_id);
    msg_type = ntohs(msg_type);
  }
};

struct connect_body {
  char msg[6];

  void to_network() {}
  void to_host() {}
};

struct ackconnect_body {
  char msg[6];

  void to_network() {}
  void to_host() {}
};

template <typename T>
int generate_msg(uint32_t session_id, uint16_t msg_type, T payload, char *data, int max_len) {
  int total_len = sizeof(struct msg_header) + sizeof(T);
  if (total_len > max_len) {
    fatalerror("generate_msg() : message too large");
  }

  struct msg_header header = {session_id, msg_type};
  header.to_network();
  payload.to_network();

  // Insert header at start of message
  memcpy(data, &header, sizeof(header));
  // Insert payload directly after
  memcpy(data + sizeof(header), &payload, sizeof(T));
  return total_len;
}

struct msg_header parse_header(char *data, int len) {
  if (len < sizeof(struct msg_header)) {
    // Ignore this message
    fatalerror("parse_header() : msg too small");
  }

  struct msg_header header;
  memcpy(&header, data, sizeof(struct msg_header));
  header.to_host();

  return header;
}

template <typename T>
T parse_body(char *data, int len) {
  if (len != sizeof(struct msg_header) + sizeof(T)) {
    fatalerror("parse_body() : invalid size");
  }

  char *payload = data + sizeof(struct msg_header);
  T body;
  memcpy(&body, payload, sizeof(body));
  body.to_host();

  return body;
}
