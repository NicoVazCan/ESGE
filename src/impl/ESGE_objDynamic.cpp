#include "ESGE_objDynamic.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

ESGE_ObjDynamic *ESGE_ObjDynamic::list = NULL;

ESGE_ObjDynamic::ESGE_ObjDynamic(
  SDL_Point pos,
  SDL_Point vel,
  SDL_Point acc,
  SDL_Rect colBox
):
  ESGE_ObjPoint(pos),
  ESGE_ObjMove(pos, vel, acc),
  ESGE_ObjCollider(pos, colBox)
{
  SGLIB_LIST_ADD(
    ESGE_ObjDynamic,
    list,
    this,
    next
  );
}

ESGE_ObjDynamic::~ESGE_ObjDynamic(void)
{
  SGLIB_LIST_DELETE(
    ESGE_ObjDynamic,
    list,
    this,
    next
  );
}
