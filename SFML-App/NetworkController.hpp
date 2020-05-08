//
//  NetworkController2.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/4/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef NetworkController_hpp
#define NetworkController_hpp

#include "PlayerInput.hpp"

#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <chrono>
#include <list>

class NetworkController {
public:
    static const int RollbackFrames = 10;
    
    typedef enum NetworkControllerState {
        OFF, WAITING, POLLING, CONNECTED, INVALID
    } NetworkControllerState;
    
    typedef struct InputData {
        int frame = -1;
        bool isPlayerValid = false;
        PlayerInput player = PlayerInput();
        bool isRemoteValid = false;
        PlayerInput remote = PlayerInput();
    } InputData;
    
public:
    NetworkController() {
        state_ = OFF;
        
        if (config == 1) {
            state_ = WAITING;
            bindIp_ = "10.0.0.222";
            bindPort_ = 59975;
        } else if (config == 2) {
            state_ = POLLING;
            bindIp_ = "10.0.0.222";
            bindPort_ = 63755;
            sendIp_ = bindIp_;
            sendPort_ = 59975;
        } else if (config == 3) {
            state_ = OFF;
        }
        
        if (socket_.bind(bindPort_, bindIp_) != sf::Socket::Done) {
            std::cerr << "ERROR BINDING TO SOCKET PORT" << std::endl;
        } else {
            std::cout << "Bound to port " << socket_.getLocalPort() << std::endl;
        }
        socket_.setBlocking(false);
        
        for (int i = 0; i < RollbackFrames * 2; i++) {
            inputData_[i].frame = -1;
        }
    }
    
    bool IsConnected() { return state_ == CONNECTED; }
    
    void SetState(NetworkControllerState state) {
        if (state == CONNECTED) {
            std::cerr << "Cannot set network controller state to " << state << std::endl;
            return;
        }
        state_ = state;
    }
    
    // Returns a positive integer if a frame is received
    bool PreTick();
        
    // Checks for remote input in the socket
    bool CheckForRemoteInput();
    
    // Sends the player input, if connected to another host. Also records the input
    void SendPlayerInput(const PlayerInput &input);
    
    // Gets the player input for the given frame. Each frame must call SendPlayerInput
    // to have it recorded.
    // should have valid frames for [0, RollbackFrames-1]
    std::list<InputData>* GetInputData() {
        std::list<InputData> *res = new std::list<InputData>();
        for (int i = 0; i < RollbackFrames; i++) {
            int index = localFrameIndex_ - 1 - i;
            if (index < 0) {
                index += RollbackFrames * 2;
            } else if (index >= RollbackFrames * 2) {
                index -= RollbackFrames * 2;
            }
            InputData d = inputData_[index];
            if (d.frame == -1) break;
            
            if (frame_ > RollbackFrames && IsConnected() && i == RollbackFrames - 1 && !d.isRemoteValid) {
                std::cerr << "DESYNCED " << frame_ << std::endl;
                assert(false);
            }
            
            assert(d.isPlayerValid);
            
            if (!d.isRemoteValid) {
                nextRemoteIndex_ = d.frame;
            }
            
            res->push_front(d);
        }
        return res;
    }
    
public:
    bool RemoteValid;
    
private:
    void Connect();
    
    // Returns false if an error occurs while calculating
    bool CalculatePing();
    bool SendPing();
    
    typedef struct InputDataPacket {
        int frame;
        int nframe;
        float xaxis;
        float yaxis;
        int buttonlen;
    } InputPacket;
    
private:
    int config = 2;
    sf::UdpSocket socket_;
    
    sf::IpAddress bindIp_ = sf::IpAddress::Any;
    unsigned short bindPort_ = sf::Socket::AnyPort;
    sf::IpAddress sendIp_;
    unsigned short sendPort_;
    
    InputData inputData_[RollbackFrames * 2];
    int localFrameIndex_ = -1;
    int nextRemoteIndex_ = 0;
    
    NetworkControllerState nextState_ = INVALID;
    NetworkControllerState state_;
    
    int frame_ = -1;
 
    friend class GameController;
    friend int main(int, char const**);
};

#endif /* NetworkController_hpp */
