#ifndef ESGE_OBJ_CAM_H_
# define ESGE_OBJ_CAM_H_

# include <SDL2/SDL.h>
# include "ESGE_scene.h"
# include "ESGE_objPoint.h"

# define ESGE_OBJ_CAM_WORLD_FACTOR 10

class ESGE_Display
{
  ESGE_Display(void);
  ~ESGE_Display(void);

public:
  static SDL_Renderer *renderer;
  static SDL_Point cam;

  static void Init(void);
  static void Update(void);
  static void Quit(void);

  SDL_FORCE_INLINE int
  PixelToWorld(int pixel)
  {
    return pixel*ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  SDL_FORCE_INLINE int
  SubpixelToWorld(float subpixel)
  {
    return SDL_lroundf(subpixel)*ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  
  SDL_FORCE_INLINE int
  WorldToPixel(int pixel)
  {
    return pixel/ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  SDL_FORCE_INLINE float
  WorldToSubpixel(int world)
  {
    return ((float)world)/((float)ESGE_OBJ_CAM_WORLD_FACTOR);
  }

  SDL_FORCE_INLINE SDL_Point
  WorldToDisplayPoint(SDL_Point pos)
  {
    pos.x = ESGE_Display::WorldToPixel(pos.x - cam.x);
    pos.y = ESGE_Display::WorldToPixel(pos.y - cam.y);

    return pos;
  }
  SDL_FORCE_INLINE SDL_Point
  DisplayToWorldPoint(SDL_Point pos)
  {
    pos.x = cam.x + ESGE_Display::PixelToWorld(pos.x);
    pos.y = cam.y + ESGE_Display::PixelToWorld(pos.y);

    return pos;
  }

  static void WorldDrawRect(
    SDL_Rect rect,
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a
  );
  static void DisplayDrawRect(
    SDL_Rect rect,
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a
  );
};

#endif