#include "ESGE_objEvent.h"
#include "ESGE_event.h"

void
ESGE_OnQuit(void *userdata, SDL_UNUSED SDL_Event *pEvent)
{
  ((ESGE_ObjQuitEvent*) userdata)->OnQuit();
}

ESGE_ObjQuitEvent::ESGE_ObjQuitEvent(void) {}

ESGE_ObjQuitEvent::~ESGE_ObjQuitEvent(void) {}

void
ESGE_ObjQuitEvent::OnEnable(void)
{
  SDL_assert(!ESGE_AddEventWatch(SDL_QUIT, ESGE_OnQuit, this));
  ESGE_ObjActive::OnEnable();
}

void
ESGE_ObjQuitEvent::OnDisable(void)
{
  ESGE_DelEventWatch(SDL_QUIT, ESGE_OnQuit, this);
  ESGE_ObjActive::OnDisable();
}

void
ESGE_ObjQuitEvent::OnQuit(void) {}


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

ESGE_ObjKeyEvent::ESGE_ObjKeyEvent(void):
  ESGE_ObjActive(),
  windowID(0u)
{}

ESGE_ObjKeyEvent::ESGE_ObjKeyEvent(Uint32 windowID):
  ESGE_ObjActive(), 
  windowID(windowID)
{}

ESGE_ObjKeyEvent::~ESGE_ObjKeyEvent(void) {}

void
ESGE_ObjKeyEvent::OnEnable(void)
{
  SDL_assert(!ESGE_AddEventWatch(SDL_KEYDOWN, ESGE_OnKey, this));
  SDL_assert(!ESGE_AddEventWatch(SDL_KEYUP,   ESGE_OnKey, this));
  ESGE_ObjActive::OnEnable();
}

void
ESGE_ObjKeyEvent::OnDisable(void)
{
  ESGE_DelEventWatch(SDL_KEYDOWN, ESGE_OnKey, this);
  ESGE_DelEventWatch(SDL_KEYUP,   ESGE_OnKey, this);
  ESGE_ObjActive::OnDisable();
}

void
ESGE_ObjKeyEvent::OnKeyDown(
  SDL_UNUSED SDL_Keycode key,
  SDL_UNUSED SDL_Keymod mod
) {}

void
ESGE_ObjKeyEvent::OnKeyUp(
  SDL_UNUSED SDL_UNUSED SDL_Keycode key,
  SDL_UNUSED SDL_Keymod mod
) {}
