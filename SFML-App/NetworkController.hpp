//
//  NetworkController.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/1/20.
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

using std::cout;
using std::cerr;
using std::endl;
using namespace std::chrono;

class NetworkController {
public:
    static const int RollbackFrames = 20;
    
    typedef enum NetworkControllerState {
        WAITING, POLLING, CONNECTED
    } NetworkState;
    
public:
    NetworkController() {
        cout << sf::IpAddress::getLocalAddress() << endl;
        if (config == 1) {
            bindPort = 59778;
            sendPort = 61342;
            recipient = "10.0.0.222";
            local = recipient;
            state = WAITING;
        } else if (config == 2) {
            bindPort = 61342;
            sendPort = 59778;
            recipient = "10.0.0.222";
            local = recipient;
            state = POLLING;
        }
        
        if (socket_.bind(bindPort, local) != sf::Socket::Done) {
            cerr << socket_.bind(bindPort, local) << endl;
            cerr << "ERROR BINDING TO SOCKET PORT" << endl;
        } else {
            cout << "BOUND TO PORT " << socket_.getLocalPort() << endl;
        }
        socket_.setBlocking(false);
        
        for (int i = 0; i < RollbackFrames; i++) {
            remoteInputs_[i] = std::make_pair(-1, PlayerInput());
        }
    }
    
    ~NetworkController() {
        socket_.unbind();
    }
    
    void SendInput(const PlayerInput &input);
    int CheckForRemoteInput();
    PlayerInput GetInput(int frame);
                    
    void Connect();
    
public:
    NetworkState state = POLLING;
    
private:
    void InsertRemoteInput(int frame, PlayerInput input);
    
    void CalculatePing();
    void SendPing();
    
private:
    int frame_ = 0;
    sf::UdpSocket socket_;
    
    int inputsIndex_;
    std::pair<int, PlayerInput> remoteInputs_[RollbackFrames];
    
    unsigned short bindPort;
    unsigned short sendPort;
    sf::IpAddress recipient;
    sf::IpAddress local;
    
    int config = 2;
    
    high_resolution_clock::time_point pingTime_;
    
    friend class GameController;
};

#endif /* NetworkController_hpp */
