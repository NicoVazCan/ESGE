#include "ESGE_objUpdate.h"

#include <SDL2/SDL.h>
#define SGLIB_ASSERT SDL_assert
#include "sglib.h"


ESGE_ObjUpdate *ESGE_ObjUpdate::list = NULL;


void
ESGE_ObjUpdate::Update(void)
{
  for (ESGE_ObjUpdate *obj = list; obj != NULL; obj = obj->next)
    obj->OnUpdate();
}


ESGE_ObjUpdate::ESGE_ObjUpdate(void)  {}
ESGE_ObjUpdate::~ESGE_ObjUpdate(void) {}


void
ESGE_ObjUpdate::EnableUpdate(void)
{
  enabledUpdate = true;
  SGLIB_LIST_ADD(ESGE_ObjUpdate, list, this, next);
}

void
ESGE_ObjUpdate::DisableUpdate(void)
{
  enabledUpdate = false;
  SGLIB_LIST_DELETE(ESGE_ObjUpdate, list, this, next);
}

bool
ESGE_ObjUpdate::IsEnabledUpdate(void)
{
  return enabledUpdate;
}


void
ESGE_ObjUpdate::OnUpdate(void)
{}
