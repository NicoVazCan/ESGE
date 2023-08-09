#include "ESGE_objPhysic.h"

#include <SDL2/SDL.h>
#define SGLIB_ASSERT SDL_assert
#include "sglib.h"


ESGE_ObjPhysic *ESGE_ObjPhysic::list = NULL;


void
ESGE_ObjPhysic::Physic(void)
{
  for (ESGE_ObjPhysic *obj = list; obj != NULL; obj = obj->next)
    obj->OnPhysic();
}


ESGE_ObjPhysic::ESGE_ObjPhysic(void)
{}

ESGE_ObjPhysic::~ESGE_ObjPhysic(void)
{}


void
ESGE_ObjPhysic::EnablePhysic(void)
{
  enabledPhysic = true;
  SGLIB_LIST_ADD(ESGE_ObjPhysic, list, this, next);
}

void
ESGE_ObjPhysic::DisablePhysic(void)
{
  enabledPhysic = false;
  SGLIB_LIST_DELETE(ESGE_ObjPhysic, list, this, next);
}

bool
ESGE_ObjPhysic::IsEnabledPhysic(void)
{
  return enabledPhysic;
}


void
ESGE_ObjPhysic::OnPhysic(void)
{}
