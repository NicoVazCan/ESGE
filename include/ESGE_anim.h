/**
 * \file ESGE_anim.h
 * \brief Contains the definitions of animation-related structures and classes.
 */

#ifndef ESGE_ANIM_H_
# define ESGE_ANIM_H_

# include <SDL2/SDL.h>
# include "ESGE_display.h"
# include "ESGE_spritesheet.h"


/**
 * \struct ESGE_Frm
 * \brief Represents a frame in an animation.
 */
struct ESGE_Frm
{
  int col; /**< Column index of the frame in the spritesheet. */
  int row; /**< Row index of the frame in the spritesheet. */
  double scale; /**< Scaling factor of the frame. */
  double angle; /**< Rotation angle of the frame. */
  SDL_Point center; /**< Rotation center of the frame. */
  SDL_RendererFlip flip; /**< Flip mode of the frame. */
  Uint32 dur; /**< Duration of the frame in milliseconds. */
};

/**
 * \struct ESGE_Anim
 * \brief Represents an animation composed of frames.
 */
struct ESGE_Anim
{
  const ESGE_Frm *frms; /**< Pointer to an array of frames. */
  size_t nFrms; /**< Number of frames in the animation. */
  int rep; /**< Number of times the animation should repeat or -1 to infinitely */
};

/**
 * \struct ESGE_AnimPlayer
 * \brief Manages playing animations using spritesheets.
 */
struct ESGE_AnimPlayer
{
  ESGE_Spritesheet *sprts; /**< Pointer to the spritesheet for rendering. */
  const ESGE_Anim *anim; /**< Pointer to the current animation. */
  size_t frm; /**< Index of the current frame being displayed. */
  Uint32 ticks; /**< Elapsed time since the current frame started playing. */
  Uint32 speed; /**< Speed of the animation playback as a percentage, 100 for normal speed. */
  int rep; /**< Remaining number of repetitions for the animation. */
  bool done; /**< Flag indicating if the animation playback is complete. */

  /**
   * \brief Start playing the given animation.
   * 
   * Initializes the animation playback and sets the provided animation as the
   * one to be played.
   * 
   * \param anim Pointer to the animation to be played.
   */
  void Start(const ESGE_Anim *anim);
  /**
   * \brief Update the animation playback.
   * 
   * Advances the animation playback based on the provided time difference
   * since the last update.
   * 
   * \param deltaTm Time difference in milliseconds since the last update.
   */
  void Update(Uint32 deltaTm);
  /**
   * \brief Check if the animation playback is complete.
   * 
   * \return True if the animation playback is complete, false otherwise.
   */
  bool IsDone(void);

  /**
   * \brief Get the sprite for the current animation frame.
   * 
   * Populates the provided sprite object with the rendering information for
   * the current frame of the animation.
   * 
   * \param sprt Pointer to the sprite object to be populated.
   */
  void GetSprite(ESGE_Sprite *sprt);
};

#endif