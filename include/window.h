#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include "graphics_headers.h"
using namespace std;

class Window
{
  public:
    //constructor
    Window();

    //deconstructor
    ~Window();

    //set up the window
    bool Initialize(const string &name, int* width, int* height);
    
    //Run the gui menu
    void RunMenu();

    //swap frames
    void Swap();

    //For menus
    MenuVars* getVars();

    //zoom of the camera
    void SetZoom(float);

    //for menus
    void setSelector(int);

    //returns angle of table
    float getAngle();

  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
    bool show_test_window;
    bool show_another_window;
    ImVec4 clear_color;

    MenuVars imgui_vars;
};

#endif /* WINDOW_H */
