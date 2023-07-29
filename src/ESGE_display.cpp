#include "ESGE_display.h"
#include "ESGE_error.h"

#define TITLE "test"
#define X SDL_WINDOWPOS_UNDEFINED
#define Y SDL_WINDOWPOS_UNDEFINED
#define W 256
#define H 144
#define WIND_FLAGS SDL_WINDOW_RESIZABLE
#define REND_INDEX -1
#define REND_FLAGS SDL_RENDERER_ACCELERATED
#define R 0
#define G 0
#define B 0
#define A 255


SDL_Renderer *ESGE_Display::renderer = NULL;
SDL_Point ESGE_Display::cam = {0, 0};


ESGE_Display::ESGE_Display(void)
{}

ESGE_Display::~ESGE_Display(void)
{}


void
ESGE_Display::Init(void)
{
  SDL_Window *window;

  SDL_assert(
    (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) ||
    !"Video was not initialized"
  );

  if (!(window = SDL_CreateWindow(TITLE, X, Y, W, H, WIND_FLAGS)))
    ESGE_Error("Cannot create window: %s", SDL_GetError());
  if (!(renderer = SDL_CreateRenderer(window, REND_INDEX, REND_FLAGS)))
    ESGE_Error("Cannot create renderer: %s", SDL_GetError());

  if (SDL_RenderSetLogicalSize(renderer, W, H))
    ESGE_Error("Cannot set logical size: %s", SDL_GetError());
}


void 
ESGE_Display::Update(void)
{
  SDL_RenderPresent(renderer);
  if (SDL_SetRenderDrawColor(renderer, R, G, B, A))
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

  rect.x = ESGE_Display::WorldToPixel(rect.x - cam.x);
  rect.y = ESGE_Display::WorldToPixel(rect.y - cam.y);
  rect.w = ESGE_Display::WorldToPixel(rect.w);
  rect.h = ESGE_Display::WorldToPixel(rect.h);

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
