#include "ESGE_objMove.h"

static void
ESGE_OnMove(void *userdata)
{
  ((ESGE_ObjMove*)userdata)->OnMove();
}

ESGE_ObjMove::ESGE_ObjMove(void)  {}
ESGE_ObjMove::~ESGE_ObjMove(void) {}

void
ESGE_ObjMove::OnEnable(void)
{
  ESGE_ObjActive::OnEnable();

  SDL_assert(!ESGE_AddPLC(ESGE_OBJ_MOVE_PRI, ESGE_OnMove, this));
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