#include "ESGE_objPoint.h"
#include "sglib.h"

ESGE_ObjPoint *ESGE_ObjPoint::list = NULL;

ESGE_ObjPoint::ESGE_ObjPoint(SDL_Point pos): pos(pos)
{
  SGLIB_LIST_ADD(ESGE_ObjPoint, list, this, next);
}

ESGE_ObjPoint::~ESGE_ObjPoint(void)
{
  SGLIB_LIST_DELETE(ESGE_ObjPoint, list, this, next);
}
