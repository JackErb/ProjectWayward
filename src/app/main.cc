#include <timer.h>
#include <GameController.h>

#include <glad/glad.h>
#include <SDL.h>

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

bool initSdl(SDL_Window**, SDL_GLContext*, int, int);

int main(int, char**) {
	bool quit = false;
    
	SDL_Window *window;
	SDL_GLContext gl_context;
    const int WIDTH = 1200;
    const int HEIGHT = 1000;
	if (!initSdl(&window, &gl_context, WIDTH, HEIGHT)) {
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

void printSdlError(string msg) {
	cerr << msg << endl;
	cerr << SDL_GetError() << endl;
}

bool initSdl(SDL_Window **window, SDL_GLContext *gl_context, int WIDTH, int HEIGHT) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { 
        printSdlError("Failed to initialize SDL."); 
        return false; 
    } 
     
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); 
 
 	// Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

  #ifdef __APPLE__
	SDL_GL_SetAttribute( // required on Mac OS
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
    );
  #endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL
        | SDL_WINDOW_SHOWN
        | SDL_WINDOW_ALLOW_HIGHDPI
    );
    *window = SDL_CreateWindow("Wayward", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        					   WIDTH, HEIGHT, window_flags);
	if (*window == NULL) {
		printSdlError("Window could not be created.");
        return false;
    }

    *gl_context = SDL_GL_CreateContext(*window);
 	if (*gl_context == NULL) {
		printSdlError("GL Context could not be created.");
		return false;
	}
 	SDL_GL_MakeCurrent(*window, *gl_context);

    // enable VSync
    SDL_GL_SetSwapInterval(1);

    cout << "SDL succesfully initialized" << endl; 
    return true; 
}
