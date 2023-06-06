#include "ESGE_objDisplay.h"

#define TITLE "test"
#define X SDL_WINDOWPOS_UNDEFINED
#define Y SDL_WINDOWPOS_UNDEFINED
#define W 160
#define H 160
#define WIND_FLAGS SDL_WINDOW_RESIZABLE
#define REND_INDEX -1
#define REND_FLAGS SDL_RENDERER_ACCELERATED
#define VSYNC 1


ESGE_ObjDisplay::ESGE_ObjDisplay(void)
{
  SDL_Window *wind;

  if ((wind = SDL_CreateWindow(TITLE, X, Y, W, H, WIND_FLAGS)) == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", SDL_GetError());
    SDL_assert("Error while creating window" != NULL);
  }
  if ((rend = SDL_CreateRenderer(wind, REND_INDEX, REND_FLAGS)) == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
    SDL_assert("Error while creating window renderer" != NULL);
  }
  if (SDL_RenderSetLogicalSize(rend, W, W))
  {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
    SDL_assert("Error while setting reder logical size" != NULL);
  }
  if (SDL_RenderSetVSync(rend, VSYNC))
  {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
    SDL_ClearError();
  }
}

ESGE_ObjDisplay::~ESGE_ObjDisplay(void)
{
  SDL_DestroyWindow(SDL_RenderGetWindow(rend));
  SDL_DestroyRenderer(rend);
}
