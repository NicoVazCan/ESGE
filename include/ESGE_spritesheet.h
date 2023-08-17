#ifndef ESGE_SPRITESHEET_H_
# define ESGE_SPRITESHEET_H_

# include <SDL2/SDL.h>
# include "ESGE_file.h"
# include "ESGE_display.h"

class ESGE_Spritesheet: public ESGE_File
{
public:
  SDL_Texture *texture;
  int w, h, spriteW, spriteH;
  SDL_Point offset;

  ESGE_Spritesheet(const char *file);
  virtual ~ESGE_Spritesheet(void);

  void GetSprite(ESGE_Sprite *sprite, int col, int row);
};

#endif