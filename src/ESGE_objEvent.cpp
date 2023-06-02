#include "ESGE_objEvent.h"
#include "ESGE_event.h"


void
ESGE_OnQuit(void *userdata, SDL_UNUSED SDL_Event *pEvent)
{
  ((ESGE_ObjQuitEvent*) userdata)->OnQuit();
}

ESGE_ObjQuitEvent::ESGE_ObjQuitEvent(void)
{
  ESGE_AddEventWatch(SDL_QUIT, ESGE_OnQuit, this);
}

ESGE_ObjQuitEvent::~ESGE_ObjQuitEvent(void)
{
  ESGE_DelEventWatch(SDL_QUIT, ESGE_OnQuit, this);
}


void
ESGE_OnKey(void *userdata, SDL_Event *pEvent)
{
  ESGE_ObjKeyEvent *pThis = (ESGE_ObjKeyEvent*) userdata;

  if (
    !pEvent->key.repeat && (
      pThis->windowID == 0u ||
      pEvent->key.windowID == pThis->windowID
    )
  )
  {
    if (pEvent->key.state == SDL_PRESSED)
      pThis->OnKeyDown(
        pEvent->key.keysym.sym,
        (SDL_Keymod) pEvent->key.keysym.mod
      );
    else
      pThis->OnKeyUp(
        pEvent->key.keysym.sym,
        (SDL_Keymod) pEvent->key.keysym.mod
      );
  }
}

ESGE_ObjKeyEvent::ESGE_ObjKeyEvent(): windowID(0u)
{
  ESGE_AddEventWatch(SDL_KEYDOWN, ESGE_OnKey, this);
  ESGE_AddEventWatch(SDL_KEYUP,   ESGE_OnKey, this);
}

ESGE_ObjKeyEvent::~ESGE_ObjKeyEvent()
{
  ESGE_DelEventWatch(SDL_KEYDOWN, ESGE_OnKey, this);
  ESGE_DelEventWatch(SDL_KEYUP,   ESGE_OnKey, this);
}
