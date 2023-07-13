#include "ESGE_objDraw.h"
#include "ESGE_render.h"

ESGE_ObjDraw::ESGE_ObjDraw(void)  {}
ESGE_ObjDraw::~ESGE_ObjDraw(void) {}

void
ESGE_OnDraw(void *userdata)
{
  ESGE_ObjDraw *this = (ESGE_ObjDraw*)userdata;

  for (
    ESGE_ObjCam *cam = ESGE_ObjCam::list;
    cam != NULL;
    cam = cam->next
  )
    this->OnDraw(drawData->cam);
}

void
ESGE_ObjDraw::OnEnable(void)
{
  ESGE_ObjActive::OnEnable();

  SDL_assert(!ESGE_AddDrawCallback(layer, ESGE_OnDraw, this));
}

void
ESGE_ObjDraw::OnDisable(void)
{
  ESGE_ObjActive::OnDisable();

  ESGE_DelDrawCallback(layer, ESGE_OnDraw, this);
}
