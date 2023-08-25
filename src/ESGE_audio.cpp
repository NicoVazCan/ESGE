#include "ESGE_audio.h"

#include "audio.h"
#include "ESGE_error.h"


Uint8 ESGE_Sound::volume;


ESGE_Sound::ESGE_Sound(const char *fileName): ESGE_File(fileName)
{
  if (!(audio = createAudio(fileName, 0, volume)))
    ESGE_Error("Cannot load \"%s\" sound", fileName);
}

ESGE_Sound::~ESGE_Sound(void)
{
  freeAudio(audio);
}


void
ESGE_Sound::Play(void)
{
  playSoundFromMemory(audio, volume);
}



Uint8 ESGE_Music::volume;


ESGE_Music::ESGE_Music(const char *fileName): ESGE_File(fileName)
{
  if (!(audio = createAudio(fileName, 1, volume)))
    ESGE_Error("Cannot load \"%s\" music", fileName);
}

ESGE_Music::~ESGE_Music(void)
{
  freeAudio(audio);
}

void
ESGE_Music::Play(void)
{
  playMusicFromMemory(audio, volume);
}


void ESGE_InitAudio(Uint8 soundVolume, Uint8 musicVolume)
{
  ESGE_Sound::volume = soundVolume;
  ESGE_Music::volume = musicVolume;

  initAudio();
}

void ESGE_QuitAudio(void)
{
  endAudio();
}


void ESGE_PauseAudio(void)
{
  pauseAudio();
}

void ESGE_UnpauseAudio(void)
{
  unpauseAudio();
}
