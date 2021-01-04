set(SOURCES rbnetwork.cc input_buffer.h input_buffer.cc network_controller.cc udp_socket.cc)

add_library(wayward_network STATIC ${SOURCES})
target_include_directories(wayward_network PRIVATE .)
