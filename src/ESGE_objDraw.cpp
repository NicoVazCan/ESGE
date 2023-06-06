#include "ESGE_objDraw.h"
#include "ESGE_render.h"

ESGE_ObjDraw::ESGE_ObjDraw(
  unsigned layer,
  SDL_Renderer *rend
): layer(layer), rend(rend)
{
  PlayDraw();
}

ESGE_ObjDraw::~ESGE_ObjDraw(void)
{
  StopDraw();
}

void
ESGE_OnDraw(void *userdata, SDL_Renderer *rend)
{
  ESGE_ObjDraw *pThis = (ESGE_ObjDraw*) userdata;

  pThis->OnDraw(rend);
}

void
ESGE_ObjDraw::PlayDraw(void)
{
  SDL_assert(!ESGE_AddDrawCallback(layer, rend, ESGE_OnDraw, this));
  drawing = SDL_TRUE;
}

void
ESGE_ObjDraw::StopDraw(void)
{
  ESGE_DelDrawCallback(layer, rend, ESGE_OnDraw, this);
  drawing = SDL_FALSE;
}

SDL_bool
ESGE_ObjDraw::IsDrawing(void) const
{
  return drawing;
}
