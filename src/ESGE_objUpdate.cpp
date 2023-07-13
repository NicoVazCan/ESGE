#include "ESGE_objUpdate.h"

static void ESGE_OnUpdate(void *userdata)
{
  ((ESGE_ObjUpdate*)userdata)->OnUpdate();
}

ESGE_ObjUpdate::ESGE_ObjUpdate(void)  {}
ESGE_ObjUpdate::~ESGE_ObjUpdate(void) {}

void ESGE_ObjUpdate::OnEnable(void)
{
  ESGE_ObjActive::OnEnable();

  ESGE_AddPLC(ESGE_OBJ_UPDATE_PRI, ESGE_OnUpdate, this);
}

void ESGE_ObjUpdate::OnDisable(void)
{
  ESGE_ObjActive::OnDisable();

  ESGE_DelPLC(ESGE_OBJ_UPDATE_PRI, ESGE_OnUpdate, this);
}
