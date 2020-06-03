
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <list>
#include <map>
#include <iostream>
#include <chrono>
#include <thread>

#include "OfflineGameController.hpp"
#include "GameController.hpp"
#include "Event.hpp"
#include "PlayerInput.hpp"

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

using std::list;
using std::map;
using std::cout;
using std::cerr;
using std::endl;
using namespace std::chrono;

static void UpdateControllerState(PlayerInput *input, unsigned int c);

int main(int, char const**)
{
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
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    GameController controller(WIDTH, HEIGHT);
    
    // Contains the state of the controller
    PlayerInput p1;
    PlayerInput p2;

    // Start the game loop
    while (window.isOpen()) {
        auto start = high_resolution_clock::now();
        
        // Clear screen
        window.clear();
        
        NetworkController *n = &controller.network_;
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
        p1.Tick();
        p2.Tick();
        sf::Joystick::update();
        UpdateControllerState(&p1, 0);
        //UpdateControllerState(&p2, 1);
        
        if (p1.IsPressed(2)) {
            pause = !pause;
        }
        
        
        
        /* ************************** */
        /* GAME CONTROLLER PROCESSING */
        /* ************************** */
        if (!pause && n->dropFrames_ == 0) {
            if (!controller.network_.PauseAndWait) {
                controller.PreTick();
                controller.ProcessInput(p1, p2);
                controller.Tick();
            } else {
                controller.network_.CheckForRemoteInput();
                cout << "Waiting for remote input..." << endl;
            }
        } else if (pause) {
            // Paused
            
            if (focus) {
                if (p1.IsPressed(3)) {
                    controller.RollbackTick();
                } else if (p1.IsPressed(0)) {
                    controller.RollbackAndReplay();
                } else if (p1.IsPressed(1)) {
                    controller.PreTick();
                    controller.ProcessInput(p1, p2);
                    controller.Tick();
                }
            }
        } else {
            n->CheckForRemoteInput();
            n->dropFrames_--;
        }
        
        // Update the window
        controller.Render(&window);
        window.display();
        
        auto now = high_resolution_clock::now();
        while (duration_cast<microseconds>(now - start).count() < 14500) {
            std::this_thread::sleep_for(microseconds(100));
            now = high_resolution_clock::now();
        }
        
        while (duration_cast<microseconds>(now - start).count() < 16700) {
            std::this_thread::sleep_for(microseconds(0));
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

void UpdateControllerState(PlayerInput *input, unsigned int c) {
    if (sf::Joystick::isConnected(c)) {
        // Check the controller's buttons
        for (int i = 0; i < sf::Joystick::getButtonCount(c); i++) {
            bool contains = input->buttons.find(i) != input->buttons.end();
            if (sf::Joystick::isButtonPressed(c, i) && !contains) {
                // This button was just pressed
                input->buttons[i] = PlayerInput::Pressed;
                // cout << "Button: " << i << std::endl;
            } else if (!sf::Joystick::isButtonPressed(c, i) && contains) {
                input->buttons[i] = PlayerInput::Released;
            }
        }
        
        // Check the controller's sticks
        input->stick = {sf::Joystick::getAxisPosition(c, sf::Joystick::X),
                       sf::Joystick::getAxisPosition(c, sf::Joystick::Y)};
    } else {
        cerr << "Controller not connected" << endl;
    }
}
