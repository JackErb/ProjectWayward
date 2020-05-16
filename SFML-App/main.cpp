
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

private void UpdateControllerState(PlayerInput *input, unsigned int c);

int main(int, char const**)
{
    const float WIDTH = 2200;
    const float HEIGHT = 1600;
    
    bool pause = false;
    bool focus = true;
    
    int i = 0;
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Game");
    window.setFramerateLimit(60);

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    GameController controller(WIDTH, HEIGHT);
    
    // Contains the state of the controller
    PlayerInput playerInput = PlayerInput();

    // Start the game loop
    while (window.isOpen()) {
        auto start = high_resolution_clock::now();
        
        // Clear screen
        window.clear();
        
        NetworkController *n = &controller.network_;
		if (n->IsConnected() && n->frame_ != 0 && n->frame_ % n->dropFramesPeriod_ == 0) {
			float drop = ((float)n->rlSum_ / n->rlCount_) - ((float)n->lSum_ / n->lCount_);
			cout << "Lag " << drop << endl;
			if (drop > 0) {
				n->dropFrames_ = (int)drop;
			}
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
        playerInput.Tick();
        sf::Joystick::update();
        UpdateControllerState(&playerInput, 0);
        
        if (playerInput.IsPressed(2)) {
            pause = !pause;
        }
        
        
        
        /* ************************** */
        /* GAME CONTROLLER PROCESSING */
        /* ************************** */
        if (!pause && n->dropFrames_ == 0) {
            if (!controller.network_.PauseAndWait) {
                controller.PreTick();
                int idx = controller.network_.localFrameIndex_;
                if (focus) {
                    controller.ProcessInput(playerInput, controller.network_.inputData_[idx].remote);
                } else {
                    controller.ProcessInput(PlayerInput(), controller.network_.inputData_[idx].remote);
                }
                controller.Tick();
            } else {
                controller.network_.CheckForRemoteInput();
                cout << "Waiting for remote input..." << endl;
            }
        } else if (pause) {
            // Paused
            
            if (focus) {
                if (playerInput.IsPressed(3)) {
                    controller.RollbackTick();
                } else if (playerInput.IsPressed(0)) {
                    controller.RollbackAndReplay();
                }
            }
        } else {
            n->dropFrames_--;
        }
        
        controller.Render(&window);
        
        auto end = high_resolution_clock::now();
        //cout << duration_cast<microseconds>(end - start).count() << endl;
        
        // Update the window
        window.display();
        
        i++;
    }

    return EXIT_SUCCESS;
}

private void UpdateControllerState(PlayerInput *input, unsigned int c) {
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
        //cerr << "Controller not connected" << endl;
    }
}
