#include "ESGE_objMove.h"

void
ESGE_OnMove(void *userdata)
{
  ((ESGE_ObjMove*)userdata)->OnMove();
}

ESGE_ObjMove::ESGE_ObjMove(
  SDL_Point pos,
  SDL_Point vel,
  SDL_Point acc
):
  ESGE_ObjPoint(pos),
  vel(vel),
  acc(acc)
{}

ESGE_ObjMove::~ESGE_ObjMove(void) {}

void
ESGE_ObjMove::OnEnable(void)
{
  ESGE_ObjActive::OnEnable();

  ESGE_AddPLC(ESGE_OBJ_MOVE_PRI, ESGE_OnMove, this);
}

void
ESGE_ObjMove::OnDisable(void)
{
  ESGE_ObjActive::OnDisable();

  ESGE_DelPLC(ESGE_OBJ_MOVE_PRI, ESGE_OnMove, this);
}

void
ESGE_ObjMove::OnMove(void)
{
  pos.x += (vel.x += acc.x);
  pos.y += (vel.y += acc.y);
}