#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  std::cout << "We'll do it live!" << std::endl;
  
  Engine *engine = new Engine("Pinball", 1280, 720);
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
