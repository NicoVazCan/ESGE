#include "ESGE_spritesheet.h"

#include "ESGE_display.h"
#include "ESGE_io.h"
#include "ESGE_error.h"


#define ESGE_IMG_NAME_LEN 128

ESGE_Spritesheet::ESGE_Spritesheet(const char *fileName):
  ESGE_File(fileName)
{
  SDL_RWops *io;
  char img[ESGE_IMG_NAME_LEN];

  if (!(io = SDL_RWFromFile(fileName, "rb")))
  {
    ESGE_Error("Cannot open file \"%s\": %s",
      fileName,
      SDL_GetError()
    );
  }
  ESGE_ReadStr(io, img, ESGE_IMG_NAME_LEN);
  spriteW = ESGE_ReadU32(io);
  spriteH = ESGE_ReadU32(io);

  SDL_RWclose(io);

  texture = ESGE_Display::LoadTexture(img);
  if (SDL_QueryTexture(texture, NULL, NULL, &w, &h))
  {
    ESGE_Error("Cannot get texture size of \"%s\" BMP: %s",
      img,
      SDL_GetError()
    );
  }
  w /= spriteW;
  h /= spriteH;
}

ESGE_Spritesheet::~ESGE_Spritesheet(void)
{
  SDL_DestroyTexture(texture);
}

void
ESGE_Spritesheet::GetSprite(ESGE_Sprite *sprite, int col, int row)
{
  SDL_assert(col < w && row < h);
  sprite->texture = texture;
  sprite->clip.x = col*spriteW;
  sprite->clip.y = row*spriteH;
  sprite->clip.w = spriteW;
  sprite->clip.h = spriteH;
}