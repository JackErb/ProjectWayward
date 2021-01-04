#include <network_controller.h>
#include <logs.h>
#include <udp_socket.h>
#include <messages.h>

#include <iostream>
#include <thread>
#include <chrono>

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::unique_lock;
using std::mutex;

time_point<high_resolution_clock> timernow();

bool check_terminate(network_controller *nc) {
  unique_lock<mutex> lock(nc->lock);
  bool terminate = nc->terminate;
  return terminate;
}

enum socket_state {
  ncConnect, ncWaitForConnect,
  ncGame
};

void connect(network_controller*);
void wait_for_connect(network_controller*);

void run_network_controller(network_controller *nc) {
  cprint("running network controller...");

  nc->socket = udp_socket();
  nc->state = ncConnect;
  nc->timestamp = timernow();
  
  nc->data_len = 1024;
  char data[nc->data_len + 1];
  nc->data = data;

  int port = 24240;
  if (nc->state == ncConnect) {
    port = 24240;
    nc->socket.set_send_addr(0, 24241);
    nc->session_id = 24;
  } else if (nc->state == ncWaitForConnect) {
    port = 24241;
    nc->socket.set_send_addr(0, 24240);
  }

  if (!nc->socket.bind(port)) {
    cprint("network_controller : unable to bind to port");
    return;
  }

  while (!check_terminate(nc)) {
    if (nc->state == ncConnect) {
      connect(nc);
    } else if (nc->state == ncWaitForConnect) {
      wait_for_connect(nc);
    } else {
      fatalerror("run_network_controller() : invalid connect state");
    }
    std::this_thread::sleep_for(milliseconds(0));
  }
  cprint("shutting down network controller.");
}

void connect(network_controller *nc) {
  // Check for data in socket
  int len;
  while ((len = nc->socket.receive(nc->data, nc->data_len)) != 0) {
    struct msg_header header = parse_header(nc->data, len);
    if (header.msg_type == mtAckConnect) {
      cprint("Connect ack received");

      struct ackconnect_body body = parse_body<struct ackconnect_body>(nc->data, len);
      cprint(body.msg);
    }
  }

  // Send a message periodically if no response received
  auto now = timernow();
  uint64_t time = duration_cast<milliseconds>(now - nc->timestamp).count();
  if (time > 2000) {
    struct connect_body body = {"hello"};
    len = generate_msg<struct connect_body>(nc->session_id, mtConnect, body, nc->data, nc->data_len);
    nc->socket.send(nc->data, len);
    nc->timestamp = now;
  }
}

void wait_for_connect(network_controller *nc) {
  // Check for data in socket
  int len;
  while ((len = nc->socket.receive(nc->data, nc->data_len)) != 0) {
    struct msg_header header = parse_header(nc->data, len);
    if (header.msg_type == mtConnect) {
      cprint("Connect received");
      struct connect_body body = parse_body<struct connect_body>(nc->data, len);
      cprint(body.msg);

      // acknowledge the connect
      struct ackconnect_body send = {"world"};
      len = generate_msg<struct ackconnect_body>(header.session_id, mtAckConnect, send, nc->data, nc->data_len);
      nc->socket.send(nc->data, len);

      nc->session_id = header.session_id;
    }
  }
}
