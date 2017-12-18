#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.hpp"
#include "graphics.hpp"
#include "camera.hpp"
#include "audio.hpp"

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

    //triggers a game over, 0 is lose, 1 is win
    void gameOver(int);
  
    //reads in a word from words/words.txt
    void readWordFromFile();

    //checks if a letter is in the word
    bool checkIfCharIsValid(char);

    //checks if we have already used a leter, returns true if we have
    bool checkIfUsedLetter(char);

    //updates and fills in gameWord if letter chosen is valid
    void updateWord(char);

    //checks if the word is complete
    bool checkIfWin();

    //prints the game word
    void printWord();

  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    Audio *m_audio;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    float rotation;
    float zoom;

    ShaderSelector selector;

    unsigned int gameState;

    std::string word;
    std::string endGame = " ";
    int alphabet[26] = {0};
    int numberWrong = 0;
    char* gameWord;
    
};

#endif // ENGINE_H
