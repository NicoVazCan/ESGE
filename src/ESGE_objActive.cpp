#include "ESGE_objActive.h"

ESGE_ObjActive::~ESGE_ObjActive()     {}
void
ESGE_ObjActive::OnEnable(void)        { enabled = SDL_TRUE;  }
void
ESGE_ObjActive::OnDisable(void)       { enabled = SDL_FALSE; }
SDL_bool
ESGE_ObjActive::IsEnabled(void) const { return enabled;      }
