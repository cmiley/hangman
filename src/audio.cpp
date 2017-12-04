#include "audio.h"

Audio::Audio()
{

}

Audio::~Audio()
{

}

bool Audio::Initialize()
{
  // Initialize Open AL
  device = alcOpenDevice(NULL); // open default device
  if (device != NULL) {
    context=alcCreateContext(device,NULL); // create context
    if (context != NULL) {
      alcMakeContextCurrent(context); // set active context
    }
    else return false;
  }
  else return false;

  alListenerfv(AL_POSITION,listenerPos);
  alListenerfv(AL_VELOCITY,listenerVel);
  alListenerfv(AL_ORIENTATION,listenerOri);

  alGetError(); // clear any error messages

  // Generate buffers, or else no sound will happen!
  alGenBuffers(NUM_BUFFERS, buffer);

  if(alGetError() != AL_NO_ERROR)
  {
    printf("Error initializing audio.\n");
    return false;
  }

  //alutLoadWAVFile("a.wav",&format,&data,&size,&freq);
  //alBufferData(buffer[0],format,data,size,freq);
  //alutUnloadWAV(format,data,size,freq);

  return true;
}