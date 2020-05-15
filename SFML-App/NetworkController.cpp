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

NetworkController::NetworkController() {
    state_ = WAITING;
    
    auto status = serverS_.connect("76.28.252.227", 24240);
    if (status != sf::Socket::Done) {
        cerr << "ERROR CONNECTING TO SERVER " << status << endl;
    }
    
    if (socket_.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        cerr << "ERROR BINDING TO SOCKET PORT" << endl;
    }
    
    cout << "Bound to port " << socket_.getLocalPort() << endl;
    
    socket_.setBlocking(false);
    
    for (int i = 0; i < RollbackFrames * 2; i++) {
        inputData_[i].frame = -1;
    }
}

void NetworkController::PreTick() {
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
            return;
        case CONNECTED:
        {
            if (frame_ > RollbackFrames) {
                long t = lastRemoteInput -
                        std::chrono::system_clock::now().time_since_epoch().count();
                if (t > 1000) {
                    // Haven't received input for more than a second
                    PauseAndWait = true;
                    HoldFrame = -1;
                }
            }
            
            
            CheckForRemoteInput();
            return;
        }
    }
}

void NetworkController::CheckForRemoteInput() {
    if (state_ != CONNECTED) return;
        
    sf::Packet packet;
    sf::IpAddress addr;
    unsigned short port;
        
    auto rstatus = socket_.receive(packet, addr, port);
    while (rstatus == sf::Socket::Done) {
        // Ensure this is a input packet
        std::string header;
        packet >> header;
        if (header != "Packet" || sendIp_ != addr || sendPort_ != port) {
            rstatus = socket_.receive(packet, addr, port);
            continue;
        }
        
        // Read the data from the packet, and construct a PlayerInput object
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
        
        // Insert the input into the inputData_ array
        int index = localFrameIndex_ + (data.frame - inputData_[localFrameIndex_].frame);
        if (index < 0) {
            index += RollbackFrames * 2;
        } else if (index >= RollbackFrames * 2) {
            index -= RollbackFrames * 2;
        }
        inputData_[index].frame = data.frame;
        inputData_[index].isRemoteValid = true;
        inputData_[index].remote = input;
        
        
        if (HoldFrame == data.frame || HoldFrame == -1) {
            PauseAndWait = false;
            cout << "Received remote input for hold frame. Resuming play." << endl;
        }
        
        lastRemoteInput = std::chrono::system_clock::now().time_since_epoch().count();
        
                
        packet = sf::Packet();
        rstatus = socket_.receive(packet, addr, port);
    }
    
    if (rstatus != sf::Socket::NotReady) {
        cerr << "Error while reading remote input " << rstatus << endl;
    }
    
    return;
}


void NetworkController::SendPlayerInput(const PlayerInput &input) {
    // Insert player input into inputData_
    inputData_[localFrameIndex_].frame = frame_;
    inputData_[localFrameIndex_].isPlayerValid = true;
    inputData_[localFrameIndex_].player = input;
    
    if (state_ != CONNECTED) return;

    // Send this frame's input as well as some copies of old input within the rollback
    // limit in case of packet loss
    for (int i = 0; i < RollbackFrames; i += 2) {
        if (frame_ < i) return;
        
        int j = localFrameIndex_ - i;
        if (j < 0) {
            j += RollbackFrames * 2;
        } else if (j >= RollbackFrames * 2) {
            j -= RollbackFrames * 2;
        }
                
        InputData data = inputData_[j];
        assert (inputData_[j].isPlayerValid);
        
        // Build and send the input data
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
            
            // Continually accept packets until the socket is not ready or a PingStart
            // is received.
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
                    
                    CalculatePing();
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
                cout << "Polling" << endl;
                sf::Packet packet;
                packet << "PingStart";
                if (socket_.send(packet, sendIp_, sendPort_) != sf::Socket::Done) {
                    cerr << "Error sending PingStart packet" << endl;
                    return;
                }
            }
            
            // Check for a PingStartResponse
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
                
                CalculatePing();
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
    
    nextState_ = CONNECTED;
    
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
