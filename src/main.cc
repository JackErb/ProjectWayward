#include <glad.h>
#include <SDL.h>
#include <graphics.h>
#include <input.h>

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <controller.h>

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

time_point<high_resolution_clock> timernow();
bool initSdl(SDL_Window**, SDL_GLContext*, int, int);
void reportGLError(GLenum error);

int main(int, char**) {
  bool quit = false;

  SDL_Window *window;
  SDL_GLContext gl_context;
  const int WIDTH = 1200;
  const int HEIGHT = 800;
  if (!initSdl(&window, &gl_context, WIDTH, HEIGHT)) {
      cerr << "Failed to initialize SDL. Exiting program." << endl;
      return EXIT_FAILURE;
  }

  input::Init();
  graphics::Init();

  struct graphics::window_display display = {WIDTH, HEIGHT, 0, 0, 0.4f};
  graphics::SetDisplay(display);

  GameController controller;

  long frame_time = 0;
  long render_time = 0;

  int frame = 0;
  glClearColor(0.1, 0.1, 0.2, 0.0);
  while (!quit) {
    frame++;
    auto start = timernow();

    glViewport(0, 0, WIDTH, HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) quit = true;
    }

    input::Update();
    controller.Update();

    auto now = timernow();
    frame_time += duration_cast<microseconds>(now - start).count();

    graphics::Render();

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
      reportGLError(error);
    }

    SDL_GL_SwapWindow(window);

    now = timernow();
    render_time += duration_cast<microseconds>(now - start).count();
    while (duration_cast<microseconds>(now - start).count() < 13000) {
      std::this_thread::sleep_for(milliseconds(1));
      now = timernow();
    }
    while (duration_cast<microseconds>(now - start).count() < 16666) {
      now = timernow();
    }

    if (frame % 60 == 0) {
      std::cout << "Frame time: " << frame_time / 60 << " ms" << std::endl;
      std::cout << " Render time: " << (render_time - frame_time) / 60 << " ms" << std::endl;

      frame_time = 0;
      render_time = 0;
    }
  }

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

void printSdlError(string msg) {
  cerr << msg << endl;
  cerr << SDL_GetError() << endl;
}

bool initSdl(SDL_Window **window, SDL_GLContext *gl_context, int WIDTH, int HEIGHT) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
      printSdlError("Failed to initialize SDL.");
      return false;
  }

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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

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

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    cerr << "Couldn't initialize glad" << endl;
    return false;
  }

  SDL_GL_MakeCurrent(*window, *gl_context);
  // Vsync :
  SDL_GL_SetSwapInterval(0);
  glViewport(0, 0, WIDTH, HEIGHT);

  cout << "OpenGL version " << GLVersion.major << "." << GLVersion.minor << endl;
  cout << "SDL & OpenGL succesfully initialized" << endl;
  return true;
}

void reportGLError(GLenum error) {
  switch (error) {
  case GL_NO_ERROR:
    break;
  case GL_INVALID_ENUM:
    cerr << "GL::ERROR:: GL invalid enum" << endl;
    break;
  case GL_INVALID_VALUE:
    cerr << "GL::ERROR:: GL invalid value" << endl;
    break;
  case GL_INVALID_OPERATION:
    cerr << "GL::ERROR:: GL invalid operation" << endl;
    break;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    cerr << "GL::ERROR:: GL invalid framebuffer operation" << endl;
    break;
  case GL_OUT_OF_MEMORY:
    cerr << "GL::ERROR:: GL out of memory" << endl;
    break;
  case GL_STACK_UNDERFLOW:
    cerr << "GL::ERROR:: GL stack underflow" << endl;
    break;
  case GL_STACK_OVERFLOW:
    cerr << "GL::ERROR:: GL stack overflow" << endl;
    break;
  }
}

time_point<high_resolution_clock> timernow() {
  return high_resolution_clock::now();
}
