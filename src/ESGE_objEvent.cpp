#include "ESGE_objEvent.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"


ESGE_Event::ESGE_Event(void)
{}

ESGE_Event::~ESGE_Event(void)
{}


SDL_Event ESGE_Event::event;

bool ESGE_Event::quit = false;

void
ESGE_Event::Loop(void)
{
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      quit = true;
      return;
    case SDL_KEYDOWN:
      ESGE_ObjKeyEvent::KeyDown();
      break;
    case SDL_KEYUP:
      ESGE_ObjKeyEvent::KeyUp();
      break;
    default:
      break;
    }
  }
}



ESGE_ObjKeyEvent *ESGE_ObjKeyEvent::list = NULL;


ESGE_ObjKeyEvent::ESGE_ObjKeyEvent(void)  {}
ESGE_ObjKeyEvent::~ESGE_ObjKeyEvent(void) {}


void
ESGE_ObjKeyEvent::EnableKeyEvent(void)
{
  enabledKeyEvent = true;
  SGLIB_LIST_ADD(ESGE_ObjKeyEvent, list, this, next);
}

void
ESGE_ObjKeyEvent::DisableKeyEvent(void)
{
  enabledKeyEvent = false;
  SGLIB_LIST_DELETE(ESGE_ObjKeyEvent, list, this, next);
}

bool
ESGE_ObjKeyEvent::IsEnabledKeyEvent(void)
{
  return enabledKeyEvent;
}


void
ESGE_ObjKeyEvent::KeyDown(void)
{
  for (ESGE_ObjKeyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (
      !ESGE_Event::event.key.repeat && (
        !obj->windowID ||
        ESGE_Event::event.key.windowID == obj->windowID
      )
    )
    {
      obj->OnKeyDown(
        ESGE_Event::event.key.keysym.sym,
        (SDL_Keymod) ESGE_Event::event.key.keysym.mod
      );
    }
  }
}

void
ESGE_ObjKeyEvent::KeyUp(void)
{
  for (ESGE_ObjKeyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (
      !ESGE_Event::event.key.repeat && (
        !obj->windowID ||
        ESGE_Event::event.key.windowID == obj->windowID
      )
    )
    {
      obj->OnKeyUp(
        ESGE_Event::event.key.keysym.sym,
        (SDL_Keymod) ESGE_Event::event.key.keysym.mod
      );
    }
  }
}


void
ESGE_ObjKeyEvent::OnKeyDown(
  SDL_UNUSED SDL_Keycode key,
  SDL_UNUSED SDL_Keymod mod
)
{}

void
ESGE_ObjKeyEvent::OnKeyUp(
  SDL_UNUSED SDL_UNUSED SDL_Keycode key,
  SDL_UNUSED SDL_Keymod mod
)
{}
