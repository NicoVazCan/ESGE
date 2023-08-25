#include "ESGE_scene.h"
#include "ESGE_audio.h"

#define WAV_FILE_LEN 128

class ObjMusicPlayer: public ESGE_ObjScene
{
  char wavFile[WAV_FILE_LEN] = "";
  ESGE_Music *music;

public:
  static char* GetWavFile(void *obj, char *str, size_t n)
  {
    SDL_strlcpy(str, ((ObjMusicPlayer*)obj)->wavFile, n);
    return str;
  }
  static void SetWavFile(void *obj, const char *str)
  {
    SDL_strlcpy(
      ((ObjMusicPlayer*)obj)->wavFile,
      str,
      WAV_FILE_LEN
    );
  }

  virtual void OnInit(void) override
  {
    music = ESGE_FileMngr<ESGE_Music>::Watch(wavFile);
  }

  virtual void OnStart(void) override
  {
    music->Play();
    ESGE_UnpauseAudio();
  }

  virtual void OnQuit(void) override
  {
    ESGE_PauseAudio();
    ESGE_FileMngr<ESGE_Music>::Leave(music);
  }
};

ESGE_TYPE_FIELDS(
  ObjMusicPlayer,
  ESGE_FIELD(
    S,
    wavFile,
    ObjMusicPlayer::GetWavFile,
    ObjMusicPlayer::SetWavFile,
    ESGE_END_FIELD
  )
)
