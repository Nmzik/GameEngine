/*#include "SoundManager.h"



SoundManager::SoundManager()
{
 //InitializeOpenAL();
}


SoundManager::~SoundManager()
{
}

void SoundManager::InitializeOpenAL()
{
 alDevice = alcOpenDevice(NULL);
 if (!alDevice) {
  std::cerr << "Could not find OpenAL device!" << std::endl;
 }

 alContext = alcCreateContext(alDevice, NULL);
 if (!alContext) {
  std::cerr << "Could not create OpenAL context!" << std::endl;
 }

 if (!alcMakeContextCurrent(alContext)) {
  std::cerr << "Unable to make OpenAL context current!" << std::endl;
 }

 alGenSources(1, &source);
 alGenBuffers(1, &buffer);

 alSourcef(source, AL_PITCH, 1);
 alSourcef(source, AL_GAIN, 1);
 alSource3f(source, AL_POSITION, 0, 0, 0);
 alSource3f(source, AL_VELOCITY, 0, 0, 0);
 alSourcei(source, AL_LOOPING, AL_FALSE);

}*/
