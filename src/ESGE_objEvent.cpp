#include "ESGE_objEvent.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"


ESGE_Event *ESGE_Event::list = NULL;


void
ESGE_Event::Event(void)
{
  for (ESGE_Event *obj = list; obj != NULL; obj = obj->next)
    obj->OnEvent();
}


ESGE_Event::ESGE_Event(void)
{}

ESGE_Event::~ESGE_Event(void)
{}


void
ESGE_Event::EnableEvent(void)
{
  enabledEvent = true;
  SGLIB_LIST_ADD(ESGE_Event, list, this, next);
}

void
ESGE_Event::DisableEvent(void)
{
  enabledEvent = false;
  SGLIB_LIST_DELETE(ESGE_Event, list, this, next);
}

bool
ESGE_Event::IsEnabledEvent(void)
{
  return enabledEvent;
}


void
ESGE_Event::OnEvent(void)
{}



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
      !ESGE_event.key.repeat && (
        !obj->windowID ||
        ESGE_event.key.windowID == obj->windowID
      )
    )
    {
      obj->OnKeyDown(
        ESGE_event.key.keysym.sym,
        (SDL_Keymod) ESGE_event.key.keysym.mod
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
      !ESGE_event.key.repeat && (
        !obj->windowID ||
        ESGE_event.key.windowID == obj->windowID
      )
    )
    {
      obj->OnKeyUp(
        ESGE_event.key.keysym.sym,
        (SDL_Keymod) ESGE_event.key.keysym.mod
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



SDL_Event ESGE_event;

bool ESGE_quit = false;

void
ESGE_EventLoop(void)
{
  while (SDL_PollEvent(&ESGE_event))
  {
    ESGE_Event::Event();

    switch (ESGE_event.type)
    {
    case SDL_QUIT:
      ESGE_quit = true;
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
