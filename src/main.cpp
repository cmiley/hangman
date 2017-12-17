#include <iostream>

#include "engine.hpp"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  std::cout << "Welcome to Hangman!" << std::endl << std::endl;
  
  Engine *engine = new Engine("Hangman", 1280, 720);
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
