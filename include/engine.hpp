#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <AL/al.h>
#include <AL/alc.h>

#include "window.h"
#include "graphics.hpp"
#include "camera.hpp"

class Engine
{
  public:
    //constructor
    Engine(string name, int width, int height);
    
    //constructor
    Engine(string name);
    
    //deconstructor
    ~Engine();

    //Set up the engine
    bool Initialize();

    //Run the whole thing baby
    void Run();

    //Run the keyboard function to detect key presses
    void Keyboard();

    //returns the DT
    unsigned int getDT();

    //returns current time in milliseconds
    long long GetCurrentTimeMillis();

    //updates the shader selector to determine what shader to use
    void updateShaderSelector();

    //Updates state of game for game logic
    void updateGameState();

    //triggers a game over
    void gameOver();
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    float rotation;
    float zoom;

    ShaderSelector selector;

    unsigned int gameState;

};

#endif // ENGINE_H
