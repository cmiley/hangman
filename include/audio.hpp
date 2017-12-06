#ifndef PINBALL_AUDIO_H
#define PINBALL_AUDIO_H

#include "graphics_headers.hpp"
#include <AL/al.h>
#include <AL/alc.h>

#include <AL/alut.h>
#define BACKEND "alut"

#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return -1;		\
	}

class Audio
{
  public:
    Audio();
    ~Audio();
    bool Initialize();
    void Update();
    void list_audio_devices(const ALCchar *devices);
    ALenum to_al_format(short channels, short samples);

  private:
    SDL_AudioSpec wavspec;
    uint32_t wavlen;
    uint8_t *wavbuf;
    ALboolean enumeration;
		ALenum format;
    const ALCchar *devices;
    const ALCchar *defaultDeviceName = "test";
    int ret;
    char *bufferData;
    ALCdevice *device;
    ALvoid *data;
    ALCcontext *context;
    ALsizei size, freq;
    ALuint buffer, source;
//    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    ALboolean loop = AL_FALSE;
    ALCenum error;
    ALint source_state;

		std::vector<std::string> audioFiles;

//    ALCdevice* device;
//    ALCcontext *context;
//
//    ALuint  buffer[NUM_BUFFERS];
//    ALuint  source[NUM_SOURCES];
//    ALuint  environment[NUM_ENVIRONMENTS];
//
//    ALsizei size,freq;
//    ALenum  format;
//    ALvoid  *data;
};

#endif //PINBALL_AUDIO_H
