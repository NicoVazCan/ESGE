/**
 * \file ESGE_audio.h
 * \brief Contains the definitions of audio-related classes and functions.
 */
#ifndef ESGE_AUDIO_H_
# define ESGE_AUDIO_H_

# include "SDL2/SDL.h"
# include "ESGE_file.h"


/**
 * \class ESGE_Sound
 * \brief Represents a sound effect.
 */
class ESGE_Sound: public ESGE_File
{
  struct sound *audio; /**< Pointer to the sound data. */

public:
  static Uint8 volume; /**< Global volume level for sound effects. */

  /**
   * \brief Constructor.
   *
   * Initializes the sound effect from the provided file.
   * This constructor should not be called directly to ensure just a
   * instance for file. Use ESGE_FileMngr<ESGE_Sound>::Watch(\a fileName).
   *
   * \param fileName Path to the sound effect file.
   */
  ESGE_Sound(const char *fileName);
  /**
   * \brief Destructor.
   *
   * Cleans up the sound effect resources.
   * This destructor should not be called directly to ensure just a
   * instance for file. Use ESGE_FileMngr<ESGE_Sound>::Leave(\a fileName).
   */
  virtual ~ESGE_Sound(void);

  /**
   * \brief Play the sound effect.
   */
  void Play(void);
};

/**
 * \class ESGE_Music
 * \brief Represents a music track that loops.
 */
class ESGE_Music: public ESGE_File
{
  struct sound *audio; /**< Pointer to the music data. */

public:
  static Uint8 volume; /**< Global volume level for music. */

  /**
   * \brief Constructor.
   *
   * Initializes the music track from the provided file.
   * This constructor should not be called directly to ensure just a
   * instance for file. Use ESGE_FileMngr<ESGE_Music>::Watch(\a fileName).
   *
   * \param fileName Path to the music track file.
   */
  ESGE_Music(const char *fileName);
  /**
   * \brief Destructor.
   *
   * Cleans up the music track resources.
   * This destructor should not be called directly to ensure just a
   * instance for file. Use ESGE_FileMngr<ESGE_Music>::Leave(\a fileName).
   */
  virtual ~ESGE_Music(void);

  /**
   * \brief Play the music track.
   */
  void Play(void);
};


/**
 * \brief Initialize the audio subsystem with sound and music volumes.
 *
 * Initializes the audio subsystem and sets the default sound and music
 * volumes.
 *
 * \param soundVolume Initial volume for sound effects.
 * \param musicVolume Initial volume for music tracks.
 */
void ESGE_InitAudio(Uint8 soundVolume, Uint8 musicVolume);
/**
 * \brief Quit and clean up the audio subsystem.
 */
void ESGE_QuitAudio(void);

/**
 * \brief Pause audio playback.
 */
void ESGE_PauseAudio(void);
/**
 * \brief Unpause audio playback.
 */
void ESGE_UnpauseAudio(void);

#endif