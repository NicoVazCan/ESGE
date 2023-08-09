#ifndef ESGE_OBJ_CAM_H_
# define ESGE_OBJ_CAM_H_

# include <SDL2/SDL.h>


# define ESGE_CAM_SCALE 10

struct ESGE_Sprite
{
  SDL_Texture *texture;
  SDL_Rect clip;
  double scale = 1., angle = 0.;
  SDL_Point center = {0, 0}, offset = {0, 0};
  SDL_RendererFlip flip = SDL_FLIP_NONE;
};

class ESGE_Display
{
  ESGE_Display(void);
  ~ESGE_Display(void);

public:
  static SDL_Renderer *renderer;
  static SDL_Point cam;

  static void Init(const char *title, int w, int h);
  static void Update(void);
  static void Quit(void);

  SDL_FORCE_INLINE int
  PixelToWorld(int pixel)
  {
    return pixel*ESGE_CAM_SCALE;
  }
  SDL_FORCE_INLINE int
  SubpixelToWorld(float subpixel)
  {
    return subpixel*((float)ESGE_CAM_SCALE);
  }
  
  SDL_FORCE_INLINE int
  WorldToPixel(int world)
  {
    return (
      (world + ESGE_CAM_SCALE/2 - (ESGE_CAM_SCALE-1)*(world < 0)) /
      ESGE_CAM_SCALE
    );
  }
  SDL_FORCE_INLINE float
  WorldToSubpixel(int world)
  {
    return ((float)world)/((float)ESGE_CAM_SCALE);
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

  SDL_FORCE_INLINE SDL_Rect
  WorldToDisplayRect(SDL_Rect rect)
  {
    rect.x = ESGE_Display::WorldToPixel(rect.x - cam.x);
    rect.y = ESGE_Display::WorldToPixel(rect.y - cam.y);
    rect.w = ESGE_Display::WorldToPixel(rect.w);
    rect.h = ESGE_Display::WorldToPixel(rect.h);

    return rect;
  }
  SDL_FORCE_INLINE SDL_Rect
  DisplayToWorldRect(SDL_Rect rect)
  {
    rect.x = cam.x + ESGE_Display::PixelToWorld(rect.x);
    rect.y = cam.y + ESGE_Display::PixelToWorld(rect.y);
    rect.w = ESGE_Display::PixelToWorld(rect.w);
    rect.h = ESGE_Display::PixelToWorld(rect.h);

    return rect;
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

  static SDL_Texture *LoadTexture(const char *fileName);

  static void WorldDrawSprite(
    const ESGE_Sprite *sprite,
    SDL_Point pos
  );
  static void DisplayDrawSprite(
    const ESGE_Sprite *sprite,
    SDL_Point pos
  );
};

#endif