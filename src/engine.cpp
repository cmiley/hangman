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

  //m_audio->Initialize();
  gameState = 0;

  //reads a random word from word.txt
  readWordFromFile();


  //set up gameWord for use
  gameWord = new char [word.size()];
  for (int index = 0; index < word.size(); index++)
  {
    gameWord[index] = '-';
  }
  gameWord[word.size()] = '\0';

  //printWord();

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

    //update gravity angle
    m_graphics->getPhysics()->changeGravity(m_window->getAngle());

    // Update and render the graphics and gamestate
    m_graphics->Update(m_DT, m_window->getVars());
    m_graphics->Render(selector, m_window->getVars(), gameState);

    //updateGameState();

    m_window->RunMenu(gameWord, numberWrong, endGame);

    // Swap to the Window
    m_window->Swap();

    //check if guessed too many times
    if (numberWrong >= 6)
    {
      //trigger lose state
      gameOver(0);
    }

    //check if word is complete
    if (checkIfWin())
    {
      //trigger win state
      gameOver(1);
    }
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
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }

    //check if in A(4) through Z(29)
    if (m_event.key.keysym.scancode >= 4 && m_event.key.keysym.scancode <= 29)
    {
      //set it according to ascii code(minus 4 plus 97)
      char letter = m_event.key.keysym.scancode + 93;

      //std::cout << letter << std::endl;
      
      if (gameState < 6)
      {
        //if we have used it, else
        if (checkIfUsedLetter(letter))
        {
          //std::cout << "You have used this letter already!" << std::endl;
        }
        else
        {
          //set the letter to have been used
          alphabet[letter - 97] = 1;

          //if it is valid else
          if (checkIfCharIsValid(letter))
          {
            //std::cout << "Correct!" << std::endl;
            updateWord(letter);
          }
          else 
          {
            //std::cout << "Wrong, you have " << 6 - numberWrong << " tries left!" << std::endl;
            updateGameState();
            numberWrong++;
          }
        }
        //printWord();
      }
    }

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
  gameState++;
}

void Engine::gameOver(int selector)
{
  std::string tempString = "Game Over!";
  if (selector == 0)
  {
    tempString.append(" You Lose! The word was ");
    tempString.append(word);
    tempString.append("!");
    //m_running = false;
  }
  else
  {
    tempString.append(" You Win!");
    //m_running = false;
  }

  endGame = tempString;
}

void Engine::readWordFromFile()
{
  ifstream fin;

  fin.open("../words/words.txt");

  if (fin.is_open())
  {
    srand (time(NULL));
    int randomNumb = rand() % 188;
    std::string tempString;

    for (int i = 0; i < randomNumb; i++)
    {
      fin >> tempString;
    }

    word = tempString;

    //std::cout << "Your word is " << word.size() << " letters long..." << std::endl;
  }
  else
  {
    std::cerr << "Unable to open word.txt" << std::endl;
  }

  fin.close();
}

bool Engine::checkIfCharIsValid(char letter)
{
  return (word.find(letter) != std::string::npos);
}

bool Engine::checkIfUsedLetter(char letter)
{
  return alphabet[letter - 97] != 0;
}

void Engine::updateWord(char letter)
{
  for (int index = 0; index < word.size(); index++)
  {
    if (letter == word[index])
    {
      gameWord[index] = letter;
    }
  }
}

bool Engine::checkIfWin()
{
  for (int index = 0; index < word.size(); index++)
  {
    if (gameWord[index] == '-')
    {
      return false;
    }
  }
  return true;
}

void Engine::printWord()
{
  std::cout << "Current Word: ";
  for (int index = 0; index < word.size(); index++)
  {
    std::cout << gameWord[index] << ' ';
  }
  std::cout << std::endl;
}