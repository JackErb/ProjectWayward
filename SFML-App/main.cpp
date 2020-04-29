
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
        
        // Poll window for events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return EXIT_SUCCESS;
            }
        }
        
        sf::Joystick::update();
        
        // Update the player input
        playerInput.Tick();
        UpdateControllerState(&playerInput, 0);
        
        controller.ProcessInput(playerInput);
        controller.Tick();
        controller.Render(&window);
        
        // Update the window
        window.display();
        
        auto end = high_resolution_clock::now();
        //cout << duration_cast<microseconds>(end - start).count() << endl;
    }

    return EXIT_SUCCESS;
}

private void UpdateControllerState(PlayerInput *input, unsigned int c) {
    if (sf::Joystick::isConnected(0)) {
        // Check the controller's buttons
        for (int i = 0; i < sf::Joystick::getButtonCount(0); i++) {
            bool contains = input->buttons.find(i) != input->buttons.end();
            if (sf::Joystick::isButtonPressed(0, i) && !contains) {
                // This button was just pressed
                input->buttons[i] = PlayerInput::Pressed;
            } else if (!sf::Joystick::isButtonPressed(0, i) && contains) {
                input->buttons[i] = PlayerInput::Released;
            }
        }
        
        // Check the controller's sticks
        input->stick = {sf::Joystick::getAxisPosition(0, sf::Joystick::X),
                       sf::Joystick::getAxisPosition(0, sf::Joystick::Y)};
    } else {
        cerr << "Controller not connected" << endl;
    }
}
