#include "audio.hpp"

Audio::Audio()
{
  // do nothing
}

Audio::~Audio()
{
  // release buffers
}

bool Audio::Initialize()
{
  std::cout << "Initializing audio" << std::endl;
//  device = alcOpenDevice(nullptr); // open default device
//  if (device != nullptr) {
//    context=alcCreateContext(device, nullptr); // create context
//    if (context != nullptr) {
//      alcMakeContextCurrent(context); // set active context
//      return true;
//    }
//  }
//
//  return false;

  std::string file = "../audio/test.wav";

  ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

  enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
  if (enumeration == AL_FALSE)
  {
    fprintf(stderr, "enumeration extension not available\n");
    return false;
  }

  list_audio_devices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));

  std::cout << "after listing devices" << std::endl;

  if (!defaultDeviceName)
    defaultDeviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);

  device = alcOpenDevice(defaultDeviceName);
  if (!device)
  {
    fprintf(stderr, "unable to open default device\n");
    return false;
  }

  fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

  alGetError();

  context = alcCreateContext(device, nullptr);
  if (!alcMakeContextCurrent(context))
  {
    fprintf(stderr, "failed to make default context\n");
    return false;
  }
  TEST_ERROR("make default context");

  /* set orientation */
  alListener3f(AL_POSITION, 0, 0, 1.0f);
  TEST_ERROR("listener position");
  alListener3f(AL_VELOCITY, 0, 0, 0);
  TEST_ERROR("listener velocity");
  alListenerfv(AL_ORIENTATION, listenerOri);
  TEST_ERROR("listener orientation");

  alGenSources((ALuint)1, &source);
  TEST_ERROR("source generation");

  alSourcef(source, AL_PITCH, 1);
  TEST_ERROR("source pitch");
  alSourcef(source, AL_GAIN, 1);
  TEST_ERROR("source gain");
  alSource3f(source, AL_POSITION, 0, 0, 0);
  TEST_ERROR("source position");
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  TEST_ERROR("source velocity");
  alSourcei(source, AL_LOOPING, AL_FALSE);
  TEST_ERROR("source looping");

  std::cout << "after sourcing" << std::endl;

  if(!SDL_LoadWAV(file.c_str(), &wavspec, &wavbuf, &wavlen))
  {
    fprintf(stderr, "Unable to load sound file\n");
    return false;
  }

  alGenBuffers(1, &buffer);
  TEST_ERROR("buffer generation");

  switch(wavspec.format)
  {
    case AUDIO_U8:
    case AUDIO_S8:
      format = wavspec.channels==2 ? AL_FORMAT_STEREO8 :
               AL_FORMAT_MONO8;
      break;
    case AUDIO_U16:
    case AUDIO_S16:
      format = wavspec.channels==2 ? AL_FORMAT_STEREO16 :
               AL_FORMAT_MONO16;
      break;
    default:
      SDL_FreeWAV(wavbuf);
      return false;
  }

  alBufferData(buffer, format, wavbuf, wavlen, wavspec.freq);
  SDL_FreeWAV(wavbuf);
  TEST_ERROR("buffer copy");

  alSourcei(source, AL_BUFFER, buffer);
  TEST_ERROR("buffer binding");

  alSourcePlay(source);
  TEST_ERROR("source playing");

  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  TEST_ERROR("source state get");
  while (source_state == AL_PLAYING)
  {
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    TEST_ERROR("source state get");
  }

  std::cout << "exiting" << std::endl;

  /* exit context */
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  device = alcGetContextsDevice(context);
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  alcCloseDevice(device);

  std::cout << "Finished initalizing" << std::endl;
}

void Audio::Update()
{
  // update stuff
}

void Audio::list_audio_devices(const ALCchar *devices)
{
  const ALCchar *device = devices, *next = devices + 1;
  size_t len = 0;

  fprintf(stdout, "Devices list:\n");
  fprintf(stdout, "----------\n");
  while (device && *device != '\0' && next && *next != '\0') {
    fprintf(stdout, "%s\n", device);
    len = strlen(device);
    device += (len + 1);
    next += (len + 2);
  }
  fprintf(stdout, "----------\n");
}

ALenum Audio::to_al_format(short channels, short samples)
{
  bool stereo = (channels > 1);

  switch (samples) {
    case 16:
      if (stereo)
        return AL_FORMAT_STEREO16;
      else
        return AL_FORMAT_MONO16;
    case 8:
      if (stereo)
        return AL_FORMAT_STEREO8;
      else
        return AL_FORMAT_MONO8;
    default:
      return -1;
  }
}