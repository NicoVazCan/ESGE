/**
 * \file ESGE_spritesheet.h
 * \brief Contains the definition of the ESGE_Spritesheet class which represents a sprite sheet with individual sprites of the same shape as a grid.
 */
#ifndef ESGE_SPRITESHEET_H_
# define ESGE_SPRITESHEET_H_

# include <SDL2/SDL.h>
# include "ESGE_file.h"
# include "ESGE_display.h"


/**
 * \class ESGE_Spritesheet
 * \brief Represents a sprite sheet with individual sprites.
 * Inherits from ESGE_File for file caching using ESGE_FileMngr
 * template.
 */
class ESGE_Spritesheet: public ESGE_File
{
public:
  SDL_Texture *texture; /**< The texture of the sprite sheet. */
  int w; /**< The number of sprite columns of the sprite sheet. */
  int h; /**< The number of sprite rows of the sprite sheet. */
  int spriteW; /**< The width of the sprites in pixels. */
  int spriteH; /**< The height of the sprites in pixels. */
  SDL_Point offset; /**< Offset for positioning the sprite in the renderer. */

  /**
   * \brief Constructor for ESGE_Spritesheet.
   * This constructor should not be called directly to ensure just a
   * instance for file. Use ESGE_FileMngr<ESGE_Spritesheet>::Watch(\a fileName).
   * 
   * \param file The file name of the sprite sheet.
   */
  ESGE_Spritesheet(const char *file);
  /**
   * \brief Virtual destructor for ESGE_Spritesheet.
   * This destructor should not be called directly to ensure just a
   * instance for file. Use ESGE_FileMngr<ESGE_Spritesheet>::Leave(\a fileName).
   */
  virtual ~ESGE_Spritesheet(void);

  /**
   * \brief Get the sprite at the specified column and row.
   * The \a sprite clip and offset will be modified based on the
   * sprite sheet information and the \a col and \a row given, all
   * other sprite fields remain unchanged.
   * 
   * \param sprite Pointer to the ESGE_Sprite object to be filled with
   *        sprite information.
   * \param col The column index of the sprite in the sprite sheet.
   * \param row The row index of the sprite in the sprite sheet.
   */
  void GetSprite(ESGE_Sprite *sprite, int col, int row);
};

#endif