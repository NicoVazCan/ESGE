#include "ESGE_display.h"

#include "ESGE_error.h"


#define ESGE_WIND_X     SDL_WINDOWPOS_UNDEFINED
#define ESGE_WIND_Y     SDL_WINDOWPOS_UNDEFINED
#define ESGE_WIND_FLAGS SDL_WINDOW_RESIZABLE
#define ESGE_REND_INDEX -1
#define ESGE_REND_FLAGS SDL_RENDERER_ACCELERATED
#define ESGE_CLEAR_R    0x00
#define ESGE_CLEAR_G    0x00
#define ESGE_CLEAR_B    0x00
#define ESGE_CLEAR_A    0xFF


SDL_Renderer *ESGE_Display::renderer = NULL;
SDL_Point ESGE_Display::cam =          {0, 0};


ESGE_Display::ESGE_Display(void)
{}

ESGE_Display::~ESGE_Display(void)
{}


void
ESGE_Display::Init(const char *title, int w, int h)
{
  SDL_Window *window;

  SDL_assert(
    (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) ||
    !"Video was not initialized"
  );

  window = SDL_CreateWindow(
    title,
    ESGE_WIND_X,
    ESGE_WIND_Y,
    w,
    h,
    ESGE_WIND_FLAGS
  );
  if (!window)
    ESGE_Error("Cannot create window: %s", SDL_GetError());

  renderer = SDL_CreateRenderer(
    window,
    ESGE_REND_INDEX,
    ESGE_REND_FLAGS
  );
  if (!renderer)
    ESGE_Error("Cannot create renderer: %s", SDL_GetError());

  if (SDL_RenderSetLogicalSize(renderer, w, h))
    ESGE_Error("Cannot set logical size: %s", SDL_GetError());
}


void 
ESGE_Display::Update(void)
{
  SDL_RenderPresent(renderer);

  if (
    SDL_SetRenderDrawColor(
      renderer,
      ESGE_CLEAR_R,
      ESGE_CLEAR_G,
      ESGE_CLEAR_B,
      ESGE_CLEAR_A
    )
  )
    ESGE_Error("Cannot set renderer clear color: %s", SDL_GetError());

  if (SDL_RenderClear(renderer))
    ESGE_Error("Cannot clear renderer: %s", SDL_GetError());
}


void
ESGE_Display::Quit(void)
{
  SDL_Window *window;

  window = SDL_RenderGetWindow(renderer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}


void
ESGE_Display::WorldDrawRect(
  SDL_Rect rect,
  Uint8 r,
  Uint8 g,
  Uint8 b,
  Uint8 a
)
{
  if (SDL_SetRenderDrawColor(renderer, r, g, b, a))
    ESGE_Error("Cannot set renderer color: %s", SDL_GetError());

  rect = WorldToDisplayRect(rect);

  if (SDL_RenderDrawRect(renderer, &rect))
    ESGE_Error("Cannot draw with renderer: %s", SDL_GetError());
}

void
ESGE_Display::DisplayDrawRect(
  SDL_Rect rect,
  Uint8 r,
  Uint8 g,
  Uint8 b,
  Uint8 a
)
{
  if (SDL_SetRenderDrawColor(renderer, r, g, b, a))
    ESGE_Error("Cannot set renderer color: %s", SDL_GetError());
  if (SDL_RenderDrawRect(renderer, &rect))
    ESGE_Error("Cannot draw with renderer: %s", SDL_GetError());
}


SDL_Texture*
ESGE_Display::LoadTexture(const char *fileName)
{
  SDL_Surface *surface;
  SDL_Texture *texture;

  if (!(surface = SDL_LoadBMP(fileName)))
  {
    ESGE_Error(
      "Cannot load \"%s\" BMP: %s",
      fileName,
      SDL_GetError()
    );
  }
  if (!(texture = SDL_CreateTextureFromSurface(renderer, surface)))
  {
    ESGE_Error(
      "Cannot create texture from \"%s\" BMP: %s",
      fileName,
      SDL_GetError()
    );
  }
  SDL_FreeSurface(surface);

  return texture;
}


void
ESGE_Display::WorldDrawSprite(
  const ESGE_Sprite *sprite,
  SDL_Point pos
)
{
  SDL_Rect dstrect;

  dstrect.x = ESGE_Display::WorldToPixel(pos.x - ESGE_Display::cam.x);
  dstrect.y = ESGE_Display::WorldToPixel(pos.y - ESGE_Display::cam.y);
  dstrect.w = sprite->clip.w;// * sprite->scale;
  dstrect.h = sprite->clip.h;// * sprite->scale;

  if (
    SDL_RenderCopyEx(
      renderer,
      sprite->texture,
      &sprite->clip,
      &dstrect,
      sprite->angle,
      &sprite->center,
      sprite->flip
    )
  )
    ESGE_Error("Cannot draw with renderer: %s", SDL_GetError());
}

void
ESGE_Display::DisplayDrawSprite(
  const ESGE_Sprite *sprite,
  SDL_Point pos
)
{
  SDL_Rect dstrect;

  dstrect.x = pos.x;
  dstrect.y = pos.y;
  dstrect.w = sprite->clip.w * sprite->scale;
  dstrect.h = sprite->clip.h * sprite->scale;

  if (
    SDL_RenderCopyEx(
      renderer,
      sprite->texture,
      &sprite->clip,
      &dstrect,
      sprite->angle,
      &sprite->center,
      sprite->flip
    )
  )
    ESGE_Error("Cannot draw with renderer: %s", SDL_GetError());
}
