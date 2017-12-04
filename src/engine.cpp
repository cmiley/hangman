#include "engine.hpp"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }
  
  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

//  device = alcOpenDevice(NULL);
//  if (!device)
//  {
//    printf("The audio failed to initialize\n");
//    return false;
//  }

  gameState = 0;

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  zoom = 1.0f;
  selector = PER_FRAGMENT;
  rotation = 180.;
  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update the DT
    m_DT = getDT();

    updateShaderSelector();

    // Update and render the graphics and gamestate
    m_graphics->Update(m_DT);
    m_graphics->Render(selector, m_window->getVars());

    //updateGameState();

    m_window->RunMenu();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if(m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if(m_event.key.keysym.sym == SDLK_ESCAPE || m_event.key.keysym.sym == SDLK_q)
    {
      m_running = false;
    }

    //left arrow and right arrow, rotates the camera to the left and to the right
    else if(m_event.key.keysym.scancode == 80 || m_event.key.keysym.scancode == 79)
    {
      float rotationScale = 360.0f / m_WINDOW_WIDTH;

      if (m_event.key.keysym.scancode == 80) //the left arrow
      {
        rotation += (rotationScale * -6);
      }
      else //the right arrow
      {
        rotation += (rotationScale * 6);
      }
      
      //to get back in 0 - 360 if needed
      while(rotation > 360)
      {
        rotation -= 360.0f;
      }
      while(rotation < 0)
      {
        rotation += 360.0f;
      }
    }

    //up arrow and down arrow, zooms in and out
    else if(m_event.key.keysym.scancode == 82 || m_event.key.keysym.scancode == 81)
    {
      float zoomScale = 0.1f * zoom;

      //zoom in
      if(m_event.key.keysym.scancode == 82) // up arrow
      {
        zoom -= zoomScale;
      }
      else //down arrow
      {
        zoom += zoomScale;
      }

      //to lock distances
      if (zoom > 2)
      {
        zoom = 2;
      }
      if (zoom < 0)
      {
        zoom = 0;
      }

      m_window->SetZoom(zoom);
    }

    //space
    else if(m_event.key.keysym.scancode == 44)
    {
      m_graphics->applyForcePlunger( 1000000 );
      
    }
    //x
    else if(m_event.key.keysym.scancode == SDL_SCANCODE_X )
    {
      m_graphics->ballDrained();
      m_graphics->getPhysics()->setPosition( btVector3( 4.2,0,4), m_graphics->lookupObjectIndex("ball"));
      m_graphics->getPhysics()->setPosition( btVector3( 4.3,.1,6), m_graphics->lookupObjectIndex("plunger"));
    }

    //z left paddel
    else if(m_event.key.keysym.scancode == SDL_SCANCODE_Z )
    {
       //m_graphics->getPhysics()->applyTorque( btVector3( 0, 1000, 0), m_graphics->lookupObjectIndex("paddleL") );
    }
    else if(m_event.key.keysym.scancode == SDL_SCANCODE_C )
    {
       //m_graphics->getPhysics()->applyTorque( btVector3( 0, -1000, 0), m_graphics->lookupObjectIndex("paddleR") );
    }
  }

  //For swaping shader programs
  if (m_event.key.keysym.scancode == SDL_SCANCODE_V)
  {
  	selector = PER_VERTEX;
    m_window->setSelector(1);
  }
  if (m_event.key.keysym.scancode == SDL_SCANCODE_F)
  {
    selector = PER_FRAGMENT;
    m_window->setSelector(2);
  }
  if (m_event.key.keysym.scancode == SDL_SCANCODE_P)
  {
    selector = PASSTHROUGH;
    m_window->setSelector(0);
  }

  //For rotating around a point, right mouse button and move the mouse
  if (m_event.type == SDL_MOUSEMOTION && m_event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
  {
      float rotationScale = 360.0f / m_WINDOW_WIDTH;

      rotation += (rotationScale * m_event.motion.xrel);

      //to get back in 0 - 360 if needed
      while(rotation > 360)
      {
        rotation -= 360.0f;
      }
      while(rotation < 0)
      {
        rotation += 360.0f;
      }
  }

  //For zooming at a point, the mouse scroll wheel
  if (m_event.type == SDL_MOUSEWHEEL)
  {
    //zooming speed
    float zoomScale = 0.1f * zoom;

    //zoom in
    if(m_event.wheel.y == 1) // scroll up
    {
      //std::cout << "SCROLLING UP" << std::endl;
      zoom -= zoomScale;
    }
    //zoom out
    else if(m_event.wheel.y == -1) // scroll down
    {
      //std::cout << "SCROLLING DOWN" << std::endl;
      zoom += zoomScale;
    }

    //to lock distances
    if (zoom > 2)
    {
      zoom = 2;
    }
    if (zoom < 0)
    {
      zoom = 0;
    }

    m_window->SetZoom(zoom);
  }

  m_graphics->getCamera()->Update(rotation, m_window->getVars()->zoom);
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

void Engine::updateShaderSelector()
{
  switch (m_window->getVars()->s_selector)
  {
    case 0:
      selector = PASSTHROUGH;
      break;
    case 1:
      selector = PER_VERTEX;
      break;
    case 2:
      selector = PER_FRAGMENT;
      break;
  }
}

void Engine::updateGameState()
{

}

void Engine::gameOver()
{

}
