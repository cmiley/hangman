#ifndef PINBALL_AUDIO_H
#define PINBALL_AUDIO_H

#include "graphics_headers.hpp"
#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>

#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

class Audio
{
  public:
    Audio();
    ~Audio();
    bool Initialize();

  private:
    ALCdevice* device;
    ALCcontext *context;

    ALuint  buffer[NUM_BUFFERS];
    ALuint  source[NUM_SOURCES];
    ALuint  environment[NUM_ENVIRONMENTS];

    ALsizei size,freq;
    ALenum  format;
    ALvoid  *data;
};

#endif //PINBALL_AUDIO_H
