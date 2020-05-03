//
//  NetworkController.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/2/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "NetworkController.hpp"

#include <chrono>
#include <math.h>

using std::pair;
using std::make_pair;
using namespace std::chrono;

int NetworkController::CheckForRemoteInput() {
    frame_++;
    if (state != CONNECTED) {
        Connect();
        return -1;
    }
    
    int frameRes = -1;

    // Check if there's data in the buffer, if so, read it and update the state
    while (true) {
        sf::Packet packet;
        sf::IpAddress sender;
        unsigned short port;
        auto status = socket_.receive(packet, sender, port);
        if (status == sf::Socket::Done) {
            int frame, buttonLen;
            packet >> frame >> buttonLen;
            
            if (frame < frame_ - RollbackFrames) {
                // This is an old frame, discard it.
                continue;
            }
            
            PlayerInput res;
            for (int i = 0; i < buttonLen; i++) {
                int button, state;
                packet >> button >> state;
                res.buttons[button] = PlayerInput::ButtonState(state);
            }
            
            float xAxis, yAxis;
            packet >> xAxis >> yAxis;
            res.stick.xAxis = xAxis;
            res.stick.yAxis = yAxis;
            
            InsertRemoteInput(frame, res);
            if (frameRes == -1)
                frameRes = frame;
            else
                frameRes = frameRes < frame ? frameRes : frame;
        } else if (status != sf::Socket::NotReady) {
            cerr << "Error receiving packet" << endl;
            break;
        } else {
            // Not ready
            break;
        }
    }
    
    return frameRes;
}

void NetworkController::InsertRemoteInput(int frame, PlayerInput input) {
    int currFrame = remoteInputs_[inputsIndex_].first;
    int index = (inputsIndex_ + (frame - currFrame)) % RollbackFrames;
    remoteInputs_[index] = make_pair(frame, input);
}

PlayerInput NetworkController::GetInput(int frame) {
    int currFrame = remoteInputs_[inputsIndex_].first;
    int index = (inputsIndex_ + (frame - currFrame)) % RollbackFrames;
    if (remoteInputs_[index].first != frame) {
        // No input for this frame; backtrack until an input is found
        return PlayerInput();
    } else {
        return remoteInputs_[index].second;
    }
}

void NetworkController::SendInput(const PlayerInput &input) {
    if (state != CONNECTED) {
        return;
    }
    
    frame_++;
    
    // Send data on udp socket
    sf::Packet packet;
    
    int buttonLen = input.buttons.size();
    packet << frame_;
    packet << buttonLen;
    for (auto it = input.buttons.begin(); it != input.buttons.end(); it++) {
        int button = it->first;
        int buttonState = it->second;
        packet << button << buttonState;
    }
    packet << input.stick.xAxis;
    packet << input.stick.yAxis;

    if (socket_.send(packet, recipient, sendPort) != sf::Socket::Done) {
        std::cout << "ERROR IN SEND SOCKET" << std::endl;
        return;
    }
}

void NetworkController::Connect() {
    if (frame_ < 20) {
        // Give the game some time to simulate
        return;
    }
    
    switch (state) {
        case WAITING:
        {
            // Not connected yet
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;
            auto rstatus = socket_.receive(packet, sender, port);
            if (rstatus == sf::Socket::Done) {
                cout << "PingStart received " << sender << " " << port << endl;
                
                std::string ping;
                packet >> ping;
                if (ping != "PingStart") return;
                recipient = sender;
                sendPort = port;
                if (socket_.send(packet, recipient, sendPort) != sf::Socket::Done) {
                    cerr << "Error sending ping response packet" << endl;
                } else {
                    CalculatePing();
                    SendPing();
                    state = CONNECTED;
                    frame_ = 0;
                    
                    auto rstatus = socket_.receive(packet, sender, port);
                    if (rstatus == sf::Socket::Done) {
                        // Ping accepted by remote host!
                        cout << "Calculating ping completed." << endl;
                    } else if (rstatus != sf::Socket::NotReady) {
                        cerr << "Error receiving ping response packet" << endl;
                    }
                    socket_.setBlocking(false);
                    return;
                }
            } else if (rstatus != sf::Socket::NotReady) {
                cerr << "Error receiving packet" << endl;
            }
            return;
        }
        case POLLING:
        {
            if (frame_ % 25 == 0) {
                // Send a ping every 15 frames
                sf::Packet packet;
                packet << "PingStart";
                cout << "Pinging " << frame_ << endl;
                if (socket_.send(packet, recipient, sendPort) != sf::Socket::Done) {
                    cerr << socket_.send(packet, recipient, sendPort) << endl;
                    cerr << "Error sending ping packet" << endl;
                }
            }
            
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;
            auto rstatus = socket_.receive(packet, sender, port);
            if (rstatus == sf::Socket::Done) {
                std::string ping;
                packet >> ping;
                if (ping != "PingStart") return;
                // Ping accepted by remote host!
                cout << "Ping accepted" << endl;

                SendPing();
                CalculatePing();
                state = CONNECTED;
                frame_ = 0;
                
                socket_.setBlocking(false);
                
                cout << "Calculating ping complete." << endl;
                
                return;
            } else if (rstatus != sf::Socket::NotReady) {
                cerr << "Error receiving ping response packet" << endl;
            }
            return;
        }
        case CONNECTED:
            // This shouldn't happen.
            return;
    }
}

void NetworkController::SendPing() {
    // Send ping packet
    sf::Packet packet;
    packet << "Ping";
    if (socket_.send(packet, recipient, sendPort) != sf::Socket::Done) {
        cerr << socket_.send(packet, recipient, sendPort) << endl;
        cerr << "Error sending ping packet" << endl;
    }
    
    pingTime_ = high_resolution_clock::now();
}

void NetworkController::CalculatePing() {
    cout << "Calculating ping" << endl;
    
    socket_.setBlocking(true);
    
    int i = 0;
    const int MAX = 10;
    long total = 0;
    pingTime_ = high_resolution_clock::now();
    while (i < MAX) {
        // Receive ping packet
        sf::IpAddress remote;
        unsigned short port;
        sf::Packet packet;
        auto rstatus = socket_.receive(packet, remote, port);
        if (rstatus == sf::Socket::Done) {
            auto t = high_resolution_clock::now();
            total += duration_cast<milliseconds>(t - pingTime_).count();
            pingTime_ = t;
            SendPing();
        } else {
            cerr << "ERROR CALCULATING PING " << rstatus << endl;
            return;
        }
        i++;
    }
    
    float ping = ((float) total) / ((float) MAX);
    
    cout << "Ping: " << total << endl;
}
