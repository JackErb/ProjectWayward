#include <rbnetwork.h>
#include "input_buffer.h"
#include "network_controller.h"
#include <logs.h>
#include <thread>
#include <mutex>

using rb::rollback;
using input::player_input;
using std::vector;

using std::unique_lock;
using std::mutex;

struct input_buffer input_buffer;
int curr_frame = 0;
network_controller nc;
std::thread nc_thread;

void rb::Init() {
  {
    unique_lock<mutex> lock(nc.lock);
    nc.terminate = false;
  }
  nc_thread = std::thread(run_network_controller, &nc);
}

void rb::Shutdown() {
  {
    unique_lock<mutex> locm(nc.lock);
    nc.terminate = true;
  }
  nc_thread.join();
}


// Note that connection will need to be established prior
// to calling StartGame()
void rb::StartGame() {
  curr_frame = 0;
}

void rb::Update() {

}

void rb::VerifyPlayerInput(struct player_input input) {

}

vector<struct player_input> rb::GetInput(int frame) {

}

struct rollback rb::CheckForRollback() {

}
