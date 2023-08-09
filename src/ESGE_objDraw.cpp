#include "ESGE_objDraw.h"

#include <SDL2/SDL.h>
#define SGLIB_ASSERT SDL_assert
#include "sglib.h"


ESGE_ObjDraw *ESGE_ObjDraw::list = NULL;


void
ESGE_ObjDraw::Draw(void)
{
  for (ESGE_ObjDraw *obj = list; obj != NULL; obj = obj->next)
    obj->OnDraw();
}


ESGE_ObjDraw::ESGE_ObjDraw(void)
{}

ESGE_ObjDraw::~ESGE_ObjDraw(void)
{}


#define CMP_OBJ_DRAW(left, right) ((left)->layer - (right)->layer)

void
ESGE_ObjDraw::EnableDraw(void)
{
  enabledDraw = true;
  SGLIB_SORTED_LIST_ADD(ESGE_ObjDraw, list, this, CMP_OBJ_DRAW, next);
}

void
ESGE_ObjDraw::DisableDraw(void)
{
  enabledDraw = false;
  SGLIB_SORTED_LIST_DELETE(ESGE_ObjDraw, list, this, next);
}

bool
ESGE_ObjDraw::IsEnabledDraw(void)
{
  return enabledDraw;
}


void
ESGE_ObjDraw::OnDraw(void)
{}
