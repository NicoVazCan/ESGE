/**
 * \file ESGE_display.h
 * \brief Contains the definitions of sprite and display-related classes and functions.
 */
#ifndef ESGE_OBJ_CAM_H_
# define ESGE_OBJ_CAM_H_

# include <SDL2/SDL.h>



/**
 * \struct ESGE_Sprite
 * \brief Represents a sprite for rendering.
 */
struct ESGE_Sprite
{
  SDL_Texture *texture; /**< Pointer to the texture for rendering. */
  SDL_Rect clip; /**< Portion of the texture to be rendered. */
  double scale = 1.0; /**< Scaling factor for the sprite. */
  double angle = 0.0; /**< Rotation angle for the sprite. */
  SDL_Point center = {0, 0}; /**< Rotation center for the sprite. */
  SDL_Point offset = {0, 0}; /**< Offset for rendering the sprite. */
  SDL_RendererFlip flip = SDL_FLIP_NONE; /**< Flip mode for the sprite. */
};

/**
 * \class ESGE_Display
 * \brief Manages the display and rendering using one window with
 * singleton pattern.
 */
class ESGE_Display
{
  /**
   * \brief Private constructor (singleton).
   */
  ESGE_Display(void);
  /**
   * \brief Private destructor (singleton).
   */
  ~ESGE_Display(void);

public:
  static SDL_Renderer *renderer; /**< Pointer to the window double frame buffer renderer. */
  static SDL_Point cam; /**< Camera position in world coordinates. */

  /**
   * \brief Initialize the display.
   *
   * Initializes the display with the provided title, width, and height.
   * In case of failure, the program will be aborted.
   *
   * \param title Title of the window.
   * \param w Width of the window.
   * \param h Height of the window.
   */
  static void Init(const char *title, int w, int h);
  /**
   * \brief Update the display.
   *
   * Updates the display buffer with drawing buffer and then clears the last one with black.
   * In case of failure, the program will be aborted.
   */
  static void Update(void);
  /**
   * \brief Quit the display.
   *
   * Cleans up and closes the display.
   */
  static void Quit(void);

  /**
   * \brief Convert a world point to a display point.
   * In case of failure, the program will be aborted.
   * 
   * \param pos World coordinates.
   * \return Display coordinates.
   */
  SDL_FORCE_INLINE SDL_Point
  WorldToDisplayPoint(SDL_Point pos)
  {
    pos.x = pos.x - cam.x;
    pos.y = pos.y - cam.y;

    return pos;
  }
  /**
   * \brief Convert a display point to a world point.
   * In case of failure, the program will be aborted.
   * 
   * \param pos Display coordinates.
   * \return World coordinates.
   */
  SDL_FORCE_INLINE SDL_Point
  DisplayToWorldPoint(SDL_Point pos)
  {
    pos.x = cam.x + pos.x;
    pos.y = cam.y + pos.y;

    return pos;
  }

  /**
   * \brief Convert a world rectangle to a display rectangle.
   * In case of failure, the program will be aborted.
   * 
   * \param rect World rectangle.
   * \return Display rectangle.
   */
  SDL_FORCE_INLINE SDL_Rect
  WorldToDisplayRect(SDL_Rect rect)
  {
    rect.x = rect.x - cam.x;
    rect.y = rect.y - cam.y;

    return rect;
  }
  /**
   * \brief Convert a display rectangle to a world rectangle.
   * In case of failure, the program will be aborted.
   * 
   * \param rect Display rectangle.
   * \return World rectangle.
   */
  SDL_FORCE_INLINE SDL_Rect
  DisplayToWorldRect(SDL_Rect rect)
  {
    rect.x = cam.x + rect.x;
    rect.y = cam.y + rect.y;

    return rect;
  }

  /**
   * \brief Draw a colored rectangle in world coordinates.
   * In case of failure, the program will be aborted.
   *
   * \param rect World rectangle to draw.
   * \param r Red color component.
   * \param g Green color component.
   * \param b Blue color component.
   * \param a Alpha color component.
   */
  static void WorldDrawRect(
    SDL_Rect rect,
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a
  );
  /**
   * \brief Draw a colored rectangle in display coordinates.
   * In case of failure, the program will be aborted.
   *
   * \param rect Display rectangle to draw.
   * \param r Red color component.
   * \param g Green color component.
   * \param b Blue color component.
   * \param a Alpha color component.
   */
  static void DisplayDrawRect(
    SDL_Rect rect,
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a
  );

  /**
   * \brief Load a texture from a file.
   * This should not be used directly, use
   * EGSE_FileMngr<ESGE_Spritesheet>::Watch(\a fileName) instead.
   * In case of failure, the program will be aborted.
   *
   * \param fileName Path to the texture file.
   * \return Pointer to the loaded texture.
   */
  static SDL_Texture *LoadTexture(const char *fileName);

  /**
   * \brief Draw a sprite in world coordinates.
   * In case of failure, the program will be aborted.
   *
   * \param sprite Pointer to the sprite to draw.
   * \param pos World coordinates to draw the sprite.
   */
  static void WorldDrawSprite(
    const ESGE_Sprite *sprite,
    SDL_Point pos
  );
  /**
   * \brief Draw a sprite in display coordinates.
   * In case of failure, the program will be aborted.
   *
   * \param sprite Pointer to the sprite to draw.
   * \param pos Display coordinates to draw the sprite.
   */
  static void DisplayDrawSprite(
    const ESGE_Sprite *sprite,
    SDL_Point pos
  );
};

#endif