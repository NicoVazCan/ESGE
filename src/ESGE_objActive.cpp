#include "ESGE_objActive.h"

ESGE_ObjActive::ESGE_ObjActive(void)
{}

ESGE_ObjActive::~ESGE_ObjActive(void)
{}

void
ESGE_ObjActive::OnEnable(void)
{
  enabled = true;
}
void
ESGE_ObjActive::OnDisable(void)
{
  enabled = false;
}

bool
ESGE_ObjActive::IsEnabled(void) const
{
  return enabled;
}
