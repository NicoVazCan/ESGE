#include "ESGE_objDraw.h"
#include "ESGE_render.h"

ESGE_ObjDraw::ESGE_ObjDraw(unsigned layer): layer(layer) {}

ESGE_ObjDraw::~ESGE_ObjDraw(void) {}

void
ESGE_OnDraw(void *userdata, SDL_Renderer *rend)
{
  ESGE_ObjDraw *pThis = (ESGE_ObjDraw*) userdata;

  pThis->OnDraw(rend);
}

void
ESGE_ObjDraw::OnEnable(void)
{
  SDL_assert(!ESGE_AddDrawCallback(layer, rend, ESGE_OnDraw, this));
  ESGE_ObjActive::OnEnable();
}

void
ESGE_ObjDraw::OnDisable(void)
{
  ESGE_DelDrawCallback(layer, rend, ESGE_OnDraw, this);
  ESGE_ObjActive::OnDisable();
}

void
ESGE_ObjDraw::SetRenderer(SDL_Renderer *rend) { this->rend = rend; }
