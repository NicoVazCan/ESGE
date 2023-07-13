#include "ESGE_objActive.h"

ESGE_ObjActive::ESGE_ObjActive()  {}
ESGE_ObjActive::~ESGE_ObjActive() {}

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
void
ESGE_ObjActive::SetEnabled(bool enabled)
{
  if (!this->enabled && enabled)
  {
    this->enabled = true;
    OnEnable();
  }
  else if (this->enabled && !enabled)
  {
    this->enabled = false;
    OnDisable();
  }
}
