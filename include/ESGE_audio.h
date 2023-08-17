#ifndef ESGE_AUDIO_H_
# define ESGE_AUDIO_H_

# include "SDL2/SDL.h"
# include "ESGE_file.h"


class ESGE_Sound: public ESGE_File
{
  struct sound *audio;

public:
  static Uint8 volume;

  ESGE_Sound(const char *fileName);
  virtual ~ESGE_Sound(void);

  void Play(void);
};

class ESGE_Music: public ESGE_File
{
  struct sound *audio;

public:
  static Uint8 volume;

  ESGE_Music(const char *fileName);
  virtual ~ESGE_Music(void);

  void Play(void);
};

void ESGE_InitAudio(void);
void ESGE_QuitAudio(void);

void ESGE_PauseAudio(void);
void ESGE_UnpauseAudio(void);

#endif