#include <timer.h>
#include <SDL.h>
#include <GameController.h>

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

bool initSdl(SDL_Window**, SDL_Renderer**, int, int);

int main(int, char**) {
	bool quit = false;
    
	SDL_Window *w;
	SDL_Renderer *rd;
    const int WIDTH = 1200;
    const int HEIGHT = 1000;
	if (!initSdl(&w, &rd, WIDTH, HEIGHT)) {
		cerr << "Failed to initialize SDL. Exiting program." << endl;
		return EXIT_FAILURE;
	}
    
    long frameCount = 0;
    int frame = 0;
    
	while (!quit) {
        auto start = Timer::now();
        
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) quit = true;
		}
        
        auto now = Timer::now();
        while (duration_cast<microseconds>(now - start).count() < 14000) {
            std::this_thread::sleep_for(milliseconds(1));
            now = Timer::now();
        }

        while (duration_cast<microseconds>(now - start).count() < 16666) {
            now = Timer::now();
        }
        
        frameCount += duration_cast<microseconds>(now - start).count();
        frame++;
        if (frame % 240 == 0) {
            cout << "Average frame time: " << frameCount / 240 << endl;
            frameCount = 0;
        }
    }

    return EXIT_SUCCESS;
}

void printSDLError(string msg) {
	cerr << msg << endl;
	cerr << SDL_GetError() << endl;
}

bool initSdl(SDL_Window **w, SDL_Renderer **rd, int WIDTH, int HEIGHT) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) { 
        printSDLError("Failed to initialize SDL."); 
        return false; 
    } 
     
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); 
    *w = SDL_CreateWindow("Wayward", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                     WIDTH, HEIGHT, SDL_WINDOW_SHOWN); 
     
    if (*w == NULL) { 
        printSDLError("Window could not be created."); 
        return false; 
    } 
     
    *rd = SDL_CreateRenderer(*w, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); 
    if (*rd == NULL) { 
        printSDLError("Renderer could not be created"); 
        return false; 
    } 
     
    SDL_SetRenderDrawColor(*rd, 0, 0, 0, 255); 
    int imgFlags = IMG_INIT_PNG; 
    if (!(IMG_Init(imgFlags) & imgFlags)) { 
        printSDLError("SDL_image could not be initialized"); 
        return false; 
    } 
 
    cout << "SDL succesfully initialized" << endl; 
    return true; 
}
