#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>

#include <list>
#include <map>
#include <iostream>
#include <chrono>
#include <thread>

#include "Game/GameController.hpp"
#include "Game/PlayerInput.hpp"
#include "Game/Loaders/ResourcePath.hpp"
#include "TextureV.hpp"

using std::list;
using std::map;
using std::cout;
using std::cerr;
using std::endl;
using namespace std::chrono;

bool initSDL(int w, int h, SDL_Renderer **rdOut, SDL_Window **wOut) {
    SDL_SetMainReady();
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) < 0) {
        cerr << "Failed to initialize SDL" << endl;
        cerr << SDL_GetError() << endl;
        return false;
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            cerr << "Warning: Linear texture filtering not enabled" << endl;
        }
        
        *wOut = SDL_CreateWindow("Wayward", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
        if (*wOut == NULL) {
            cerr << "Window could not be created" << endl;
            cerr << SDL_GetError() << endl;
            return false;
        } else {
            *rdOut = SDL_CreateRenderer(*wOut, -1, SDL_RENDERER_ACCELERATED);
            if (*rdOut == NULL) {
                cerr << "Renderer could not be created" << endl;
                cerr << SDL_GetError() << endl;
                return false;
            } else {
                SDL_SetRenderDrawColor(*rdOut, 0x59, 0x82, 0xC5, 0xFF);
                
                int f = IMG_INIT_PNG;
                if (!(IMG_Init(f) & f)) {
                    cerr << "SDL image flag could not be initialized" << endl;
                    cerr << SDL_GetError() << endl;
                    return false;
                }
            }
        }
    }
    
    return true;
}

int main(int, char const**) {
    const float WIDTH = 1800;
    const float HEIGHT = 1200;
    
    SDL_Renderer *rd;
    SDL_Window *window;
    
    if (!initSDL(WIDTH, HEIGHT, &rd, &window)) {
        cerr << "Failed to initialize SDL. Closing application." << endl;
        return EXIT_FAILURE;
    }
    
    bool quit = false;
    bool pause = false;
    bool focus = true;
    
    int i = 0;
    long count = 0;
    
    GameController controller(rd, WIDTH, HEIGHT);
    NetworkController *n = &controller.network_;
    
    // Contains the state of the controller
    PlayerInput p1(0);
    PlayerInput p2(1);
    
    SDL_Event e;
    
    SpriteLoader::LoadTexture(rd, "rocket");

    // Start the game loop
    while (!quit) {
        auto start = high_resolution_clock::now();
        
        // Clear screen
        SDL_RenderClear(rd);
        
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
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            } /*else if (event.type == sf::Event::GainedFocus) {
                focus = true;
            } else if (event.type == sf::Event::LostFocus) {
                focus = false;
            }*/
        }
        
        // Update the player input
        p1.UpdateControllerState();
        //p2.UpdateControllerState();
        
        if (p1.IsPressed(A)) {
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
            if (p1.IsPressed(Y)) {
                controller.PreTick();
                controller.ProcessInput(p1, p2);
                controller.Tick();
            }
            
            if (p1.IsPressed(X)) {
                controller.RollbackTick();
            }
            
            if (p1.IsPressed(B)) {
                controller.Rollback();
            }
        }
        
        // Update the window
        controller.Render(rd);
        
        SDL_RenderPresent(rd);
        
        
        /* ************************** */
        /*   STALL UNTIL NEXT FRAME   */
        /* ************************** */
        auto now = high_resolution_clock::now();
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
