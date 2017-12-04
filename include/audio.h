#ifndef PINBALL_AUDIO_H
#define PINBALL_AUDIO_H

#include "graphics_headers.h"
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
    ALfloat listenerPos[]={0.0,0.0,4.0};
    ALfloat listenerVel[]={0.0,0.0,0.0};
    ALfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};
    ALfloat source0Pos[]={-2.0, 0.0, 0.0};
    ALfloat source0Vel[]={0.0, 0.0, 0.0};

    ALuint  buffer[NUM_BUFFERS];
    ALuint  source[NUM_SOURCES];
    ALuint  environment[NUM_ENVIRONMENTS];

    ALsizei size,freq;
    ALenum  format;
    ALvoid  *data;
};

#endif //PINBALL_AUDIO_H
