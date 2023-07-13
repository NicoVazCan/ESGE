#include "ESGE_objEditor.h"

#define TITLE "editor"
#define X SDL_WINDOWPOS_UNDEFINED
#define Y SDL_WINDOWPOS_UNDEFINED
#define W 256
#define H 144
#define WIND_FLAGS SDL_WINDOW_RESIZABLE
#define REND_INDEX -1
#define REND_FLAGS SDL_RENDERER_ACCELERATED


ESGE_ADD_TYPE(ESGE_ObjEditorCam)

ESGE_ObjEditorCam::ESGE_ObjEditorCam(void)
{
  SDL_Window *window;

  SDL_assert(
    (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) ||
    "Error: video was not initialized" == NULL
  );

  if (
    (
      window = SDL_CreateWindow(TITLE, X, Y, W, H, WIND_FLAGS)
    ) == NULL
  )
  {
    throw ESGE_VideoError();
  }
  if (
    (
      render = SDL_CreateRenderer(window, REND_INDEX, REND_FLAGS)
    ) == NULL
  )
  {
    throw ESGE_RenderError();
  }
  if (SDL_RenderSetLogicalSize(render, W, H))
  {
    throw ESGE_RenderError();
  }
}

ESGE_ObjEditorCam::~ESGE_ObjEditorCam(void)
{
  SDL_Window *window;

  window = SDL_RenderGetWindow(render);
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
}
