//
//  NetworkController2.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/4/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "NetworkController.hpp"

using std::cerr;
using std::cout;
using std::endl;
using namespace std::chrono;

bool NetworkController::PreTick() {
    if (nextState_ != INVALID) {
        state_ = nextState_;
        nextState_ = INVALID;
        
        if (state_ == CONNECTED) {
            frame_ = -1;
            localFrameIndex_ = -1;
            socket_.setBlocking(false);
            
            for (int i = 0; i < RollbackFrames * 2; i++) {
                inputData_[i].frame = -1;
                inputData_[i].isPlayerValid = false;
                inputData_[i].player = PlayerInput();
                inputData_[i].isRemoteValid = false;
                inputData_[i].remote = PlayerInput();
            }
        }
    }
    
    frame_++;
    localFrameIndex_++;
    if (localFrameIndex_ == RollbackFrames * 2) {
        localFrameIndex_ = 0;
    }
    
    inputData_[localFrameIndex_].isPlayerValid = false;
    if (inputData_[localFrameIndex_].frame != frame_) {
        // It hasn't been set yet
        inputData_[localFrameIndex_].isRemoteValid = false;
        inputData_[localFrameIndex_].remote = PlayerInput();
    }
    inputData_[localFrameIndex_].frame = frame_;
        
    switch (state_) {
        case WAITING:
        case POLLING:
            // Attempt to connect
            Connect();
        case OFF:
        case INVALID:
            return frame_ != 0 && frame_ % RollbackFrames == 0;
        case CONNECTED:
        {
            return CheckForRemoteInput();
        }
    }
}

bool NetworkController::CheckForRemoteInput() {
    if (state_ != CONNECTED) return false;
    
    int minFrame = -1;
    
    sf::Packet packet;
    sf::IpAddress addr;
    unsigned short port;
        
    auto rstatus = socket_.receive(packet, addr, port);
    while (rstatus == sf::Socket::Done) {
        std::string header;
        packet >> header;
        if (header != "Packet" || sendIp_ != addr || sendPort_ != port) {
            rstatus = socket_.receive(packet, addr, port);
            continue;
        }
        
        InputPacket data;
        packet >> data.frame >> data.nframe;
        // This is an old frame; discoard it.
        if (frame_ > RollbackFrames &&
            data.frame < inputData_[localFrameIndex_].frame - RollbackFrames) {
            rstatus = socket_.receive(packet, addr, port);
            continue;
        }
        
        packet >> data.xaxis >> data.yaxis >> data.buttonlen;
        
        PlayerInput input;
        input.stick.xAxis = data.xaxis;
        input.stick.yAxis = data.yaxis;
        for (int i = 0; i < data.buttonlen; i++) {
            int button, state;
            packet >> button >> state;
            input.buttons[button] = PlayerInput::ButtonState(state);
        }
        
        int index = localFrameIndex_ + (data.frame - inputData_[localFrameIndex_].frame);
        if (index < 0) {
            index += RollbackFrames * 2;
        } else if (index >= RollbackFrames * 2) {
            index -= RollbackFrames * 2;
        }
        cout << inputData_[index].frame << endl;
        inputData_[index].frame = data.frame;
        if (!inputData_[index].isRemoteValid) {
            cout << "Received remote input for frame " << data.frame << " on frame " << frame_ << endl;
        }
        inputData_[index].isRemoteValid = true;
        inputData_[index].remote = input;
        
        if (minFrame == -1 || data.frame < minFrame) minFrame = data.frame;
        
        packet = sf::Packet();
        rstatus = socket_.receive(packet, addr, port);
    }
    
    if (rstatus != sf::Socket::NotReady) {
        cerr << "Error while reading remote input " << rstatus << endl;
    }
    
    return minFrame != -1 && minFrame < frame_;
}


void NetworkController::SendPlayerInput(const PlayerInput &input) {
    inputData_[localFrameIndex_].frame = frame_;
    inputData_[localFrameIndex_].isPlayerValid = true;
    inputData_[localFrameIndex_].player = input;
    
    if (state_ != CONNECTED) return;

    // TODO: Send old frames in case of packet loss
    for (int i = 0; i < 5; i++) {
        if (frame_ < i) return;
        
        int j = localFrameIndex_ - i;
        if (j < 0) {
            j += RollbackFrames * 2;
        } else if (j >= RollbackFrames * 2) {
            j -= RollbackFrames * 2;
        }
                
        InputData data = inputData_[j];
        assert (inputData_[j].isPlayerValid);
        InputPacket idata = {data.frame, 0, data.player.stick.xAxis, data.player.stick.yAxis, static_cast<int>(data.player.buttons.size())};
        
        sf::Packet packet;
        packet << "Packet" << idata.frame << idata.nframe << idata.xaxis << idata.yaxis << idata.buttonlen;
        for (auto it = data.player.buttons.begin(); it != data.player.buttons.end(); it++) {
            packet << it->first << it->second;
        }
        
        if (socket_.send(packet, sendIp_, sendPort_) != sf::Socket::Done) {
            cerr << "Error sending player input" << endl;
        }
    }
}

void NetworkController::Connect() {
    switch (state_) {
        case WAITING:
        {
            if (frame_ < RollbackFrames * 2) return;
            sf::Packet packet;
            while (true) {
                auto rstatus = socket_.receive(packet, sendIp_, sendPort_);
                if (rstatus == sf::Socket::Done) {
                    std::string ping;
                    packet >> ping;
                    if (ping != "PingStart") {
                        cerr << "Waiting for PingStart, other packet received" << endl;
                        continue;
                    }
                    
                    // PingStart received!
                    sf::Packet response;
                    response << "PingStartResponse";
                    if (socket_.send(response, sendIp_, sendPort_) != sf::Socket::Done) {
                        cerr << "Error sending PingStartResponse packet" << endl;
                        return;
                    }
                    
                    cout << "PingStart received" << endl;
                    
                    if (!CalculatePing()) return;
                    nextState_ = CONNECTED;
                    return;
                } else if (rstatus != sf::Socket::NotReady) {
                    cerr << "Error receiving packet while WAITING" << endl;
                    return;
                } else {
                    break;
                }
            }
            break;
        }
        case POLLING:
        {
            // Only poll once every 25 frames
            if (frame_ % 25 == 0) {
                sf::Packet packet;
                packet << "PingStart";
                if (socket_.send(packet, sendIp_, sendPort_) != sf::Socket::Done) {
                    cerr << "Error sending PingStart packet" << endl;
                    return;
                }
            }
            
            sf::Packet response;
            sf::IpAddress addr;
            unsigned short port;
            auto rstatus = socket_.receive(response, addr, port);
            if (rstatus == sf::Socket::Done && addr == sendIp_ && port == sendPort_) {
                std::string ping;
                response >> ping;
                if (ping != "PingStartResponse") {
                    cerr << "Waiting for PingStartResponse, received other packet" << endl;
                    return;
                }
                cout << "PingStartResponse received" << endl;
                
                if (!CalculatePing()) return;
                nextState_ = CONNECTED;
                return;
            } else if (rstatus != sf::Socket::NotReady) {
                cerr << "Error receiving packet while POLLING" << endl;
                return;
            }
            break;
        }
        default:
            cerr << "ERROR Connect() called in state " << state_ << endl;
            break;
    }
}

bool NetworkController::CalculatePing() {
    cout << "Calculating ping" << endl;
       
    socket_.setBlocking(true);
    
    if (state_ == POLLING) {
        if (!SendPing()) return false;
    }
   
    int i = 0;
    const int MAX = 10;
    long total = 0;
    auto pingTime = high_resolution_clock::now();
    while (i < MAX) {
        // Receive ping packet
        sf::IpAddress remote;
        unsigned short port;
        sf::Packet packet;
        auto rstatus = socket_.receive(packet, remote, port);
        if (rstatus == sf::Socket::Done) {
            auto t = high_resolution_clock::now();
            total += duration_cast<milliseconds>(t - pingTime).count();
            pingTime = t;
            if (!SendPing()) return false;
        } else {
            cerr << "ERROR CALCULATING PING " << rstatus << endl;
            cerr << "EXITING" << endl;
            return false;
        }
        i++;
    }
    
    float ping = ((float) total) / ((float) MAX);
   
    socket_.setBlocking(false);
    cout << "Calculated Ping: " << total << endl;
    
    return true;
}

bool NetworkController::SendPing() {
    sf::Packet ping;
    ping << "Ping";
    if (socket_.send(ping, sendIp_, sendPort_) != sf::Socket::Done) {
        cerr << "Error sending ping packet" << endl;
        return false;
    }
    return true;
}
