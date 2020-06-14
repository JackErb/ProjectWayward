#include <SFML/Graphics.hpp>

#include <list>
#include <map>
#include <iostream>
#include <chrono>
#include <thread>

#include "Game/GameController.hpp"
#include "Game/PlayerInput.hpp"
#include "Game/Loaders/ResourcePath.hpp"

using std::list;
using std::map;
using std::cout;
using std::cerr;
using std::endl;
using namespace std::chrono;

int main(int, char const**) {
    const float WIDTH = 2800;
    const float HEIGHT = 1750;
    
    bool pause = false;
    bool focus = true;
    
    int i = 0;
    long count = 0;
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Game");
    window.setVerticalSyncEnabled(true);

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(ResourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    GameController controller(WIDTH, HEIGHT);
    NetworkController *n = &controller.network_;
    
    // Contains the state of the controller
    PlayerInput p1;
    PlayerInput p2;

    // Start the game loop
    while (window.isOpen()) {
        auto start = high_resolution_clock::now();
        
        // Clear screen
        window.clear();
        
		if (n->IsConnected() && n->rlCount_ >= n->dropFramesPeriod_) {
			float drop = ((float)n->lSum_ / (float)n->lCount_) - ((float)n->rlSum_ / (float)n->rlCount_);
			cout << "Lag " << drop << endl;
            if (drop > 0.75f) {
                n->dropFrames_ = (int)(drop);
			}
            n->ResetLagCounters();
		}
        
        /* ************************** */
        /* INPUT AND EVENT PROCESSING */
        /* ************************** */
        // Poll window for events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return EXIT_SUCCESS;
            } else if (event.type == sf::Event::GainedFocus) {
                focus = true;
            } else if (event.type == sf::Event::LostFocus) {
                focus = false;
            }
        }
        
        // Update the player input
        sf::Joystick::update();
        p1.UpdateControllerState(0);
        //p2.UpdateControllerState(1);
        
        if (p1.IsPressed(2)) {
            pause = !pause;
        }
        
        
        
        /* ************************** */
        /* GAME CONTROLLER PROCESSING */
        /* ************************** */
        if (!pause && n->dropFrames_ == 0) {
            if (!n->PauseAndWait) {
                controller.PreTick();
                controller.ProcessInput(p1, p2);
                controller.Tick();
            } else {
                n->CheckForRemoteInput();
                cout << "Waiting for remote input..." << endl;
            }
        } else if (!pause && n->dropFrames_ == 0) {
            // Drop a frame
            n->CheckForRemoteInput();
            n->dropFrames_--;
        } else {
            // Paused
            if (p1.IsPressed(1)) {
                controller.PreTick();
                controller.ProcessInput(p1, p2);
                controller.Tick();
            }
            
            if (p1.IsPressed(0)) {
                controller.RollbackTick();
            }
            
            if (p1.IsPressed(3)) {
                controller.Rollback();
            }
        }
        
        // Update the window
        controller.Render(&window);
        window.display();
        
        
        /* ************************** */
        /*   STALL UNTIL NEXT FRAME   */
        /* ************************** */
        auto now = high_resolution_clock::now();
        if (i % 50 == 0) {
            cout << duration_cast<microseconds>(now - start).count() << endl;
        }
        while (duration_cast<microseconds>(now - start).count() < 14000) {
            std::this_thread::sleep_for(microseconds(500));
            now = high_resolution_clock::now();
        }
        
		now = high_resolution_clock::now();
        while (duration_cast<microseconds>(now - start).count() < 16666) {
            now = high_resolution_clock::now();
        }
        
        count += (long) duration_cast<microseconds>(now - start).count();
        i++;
        
        if (i == 100) {
           cout << "Average frame time: " << count / i << endl;
           count = i = 0;
        }
    }
    return EXIT_SUCCESS;
} 
