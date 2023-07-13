#include "ESGE_objCam.h"
#define SGLIB_ASSERT SDL_assert
#include "sglib.h"

ESGE_ObjCam *ESGE_ObjCam::list = NULL;

ESGE_ObjCam::ESGE_ObjCam(void)
{
  if (!this) return;
  
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjCam,
    list,
    this,
    Cmp,
    next
  );
}

ESGE_ObjCam::~ESGE_ObjCam(void)
{
  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjCam,
    list,
    this,
    next
  );
}


SDL_Point
ESGE_ObjCam::WorldToDisplayPoint(SDL_Point pos) const
{
  pos.x = ESGE_ObjCam::WorldToPixel(pos.x - this->pos.x);
  pos.y = ESGE_ObjCam::WorldToPixel(pos.y - this->pos.y);

  return pos;
}

SDL_Point
ESGE_ObjCam::DisplayToWorldPoint(SDL_Point pos) const
{
  pos.x = this->pos.x + ESGE_ObjCam::PixelToWorld(pos.x);
  pos.y = this->pos.y + ESGE_ObjCam::PixelToWorld(pos.y);

  return pos;
}
