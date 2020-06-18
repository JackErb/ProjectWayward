#include <SDL2/SDL_render.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>

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

#define WIDTH 2800
#define HEIGHT 1750

bool initSdl(SDL_Renderer **rd, SDL_Window **w) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        cerr << "Failed to initialize SDL." << endl;
        cerr << SDL_GetError() << endl;
    } else {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        *w = SDL_CreateWindow("Wayward", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if (*w == NULL) {
            cerr << "Window could not be created" << endl;
            cerr << SDL_GetError() << endl;
        } else {
            *rd = SDL_CreateRenderer(*w, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
            if (*rd == NULL) {
                cerr << "Renderer could not be created" << endl;
                cerr << SDL_GetError() << endl;
            } else {
                SDL_SetRenderDrawColor(*rd, 0, 0, 0, 255);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    cerr << "SDL_image could not be initialized" << endl;
                    cerr << SDL_GetError() << endl;
                } else {
                    cout << "SDL succesfully initialized" << endl;
                    return true;
                }
            }
        }
    }
}

int main(int, char const**) {
    bool pause = false;
    bool focus = true;
    bool quit = false;
    
    int i = 0;
    long count = 0;
    long subFrameCount = 0;
    
    SDL_Renderer *rd;
    SDL_Window *window;
    
    if (!initSdl(&rd, &window)) {
        return EXIT_FAILURE;
    }
    
    MoveLoader::LoadMoves();
    
    GameController controller(rd, WIDTH, HEIGHT);
    NetworkController *n = &controller.network_;
    
    // Contains the state of the controller
    PlayerInput p1(0);
    PlayerInput p2(1);
    
    SDL_Event e;

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
            }
        }
        if (quit) break;
        
        // Update the player input
        p1.UpdateControllerState();
        //p2.UpdateControllerState();
        
        if (p1.IsPressed(START)) {
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
        } else if (pause) {
            // Paused
            if (p1.IsPressed(ATTACK)) {
                controller.PreTick();
                controller.ProcessInput(p1, p2);
                controller.Tick();
            }
        } else {
            // Drop a frame
            n->CheckForRemoteInput();
            n->dropFrames_--;
        }
        
        // Update the renderer and display
        controller.Render(rd);
        SDL_RenderPresent(rd);
        
        /* ************************** */
        /*   STALL UNTIL NEXT FRAME   */
        /* ************************** */
        auto now = high_resolution_clock::now();
        subFrameCount += duration_cast<microseconds>(now - start).count();
        while (duration_cast<microseconds>(now - start).count() < 14000) {
            std::this_thread::sleep_for(microseconds(500));
            now = high_resolution_clock::now();
        }
        
		now = high_resolution_clock::now();
        while (duration_cast<microseconds>(now - start).count() < 16500) {
            std::this_thread::sleep_for(microseconds(0));
            now = high_resolution_clock::now();
        }
        
        while (duration_cast<microseconds>(now - start).count() < 16666) {
            now = high_resolution_clock::now();
        }
        
        count += (long) duration_cast<microseconds>(now - start).count();
        i++;
        
        if (i == 100) {
            cout << "Avg frame time: " << count / i << endl;
            cout << "Avg subframe time: " << subFrameCount / i << endl;
            count = subFrameCount = i = 0;
        }
    }
    return EXIT_SUCCESS;
} 
