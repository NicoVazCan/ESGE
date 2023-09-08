#include "ESGE_objEvent.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_error.h"


ESGE_ObjEvent *ESGE_ObjEvent::list = NULL;


void
ESGE_ObjEvent::Event(void)
{
  for (ESGE_ObjEvent *obj = list; obj != NULL; obj = obj->next)
    obj->OnEvent();
}


ESGE_ObjEvent::ESGE_ObjEvent(void)
{}

ESGE_ObjEvent::~ESGE_ObjEvent(void)
{}


void
ESGE_ObjEvent::EnableEvent(void)
{
  enabledEvent = true;
  SGLIB_LIST_ADD(ESGE_ObjEvent, list, this, next);
}

void
ESGE_ObjEvent::DisableEvent(void)
{
  enabledEvent = false;
  SGLIB_LIST_DELETE(ESGE_ObjEvent, list, this, next);
}

bool
ESGE_ObjEvent::IsEnabledEvent(void)
{
  return enabledEvent;
}


void
ESGE_ObjEvent::OnEvent(void)
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
    if (!ESGE_event.key.repeat)
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
    if (!ESGE_event.key.repeat)
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


ESGE_ObjMouseEvent *ESGE_ObjMouseEvent::list = NULL;


ESGE_ObjMouseEvent::ESGE_ObjMouseEvent(void)  {}
ESGE_ObjMouseEvent::~ESGE_ObjMouseEvent(void) {}


void
ESGE_ObjMouseEvent::EnableMouseEvent(void)
{
  enabledMouseEvent = true;
  SGLIB_LIST_ADD(ESGE_ObjMouseEvent, list, this, next);
}

void
ESGE_ObjMouseEvent::DisableMouseEvent(void)
{
  enabledMouseEvent = false;
  SGLIB_LIST_DELETE(ESGE_ObjMouseEvent, list, this, next);
}

bool
ESGE_ObjMouseEvent::IsEnabledMouseEvent(void)
{
  return enabledMouseEvent;
}


void
ESGE_ObjMouseEvent::MouseMove(void)
{
  for (ESGE_ObjMouseEvent *obj = list; obj != NULL; obj = obj->next)
  {
      obj->OnMouseMove(
        ESGE_event.motion.x,
        ESGE_event.motion.y,
        ESGE_event.motion.xrel,
        ESGE_event.motion.yrel
      );
  }
}
void
ESGE_ObjMouseEvent::MouseButtonDown(void)
{
  for (ESGE_ObjMouseEvent *obj = list; obj != NULL; obj = obj->next)
  {
    obj->OnMouseButtonDown(
      ESGE_event.button.x,
      ESGE_event.button.y,
      ESGE_event.button.button
    );
  }
}
void
ESGE_ObjMouseEvent::MouseButtonUp(void)
{
  for (ESGE_ObjMouseEvent *obj = list; obj != NULL; obj = obj->next)
  {
    obj->OnMouseButtonUp(
      ESGE_event.button.x,
      ESGE_event.button.y,
      ESGE_event.button.button
    );
  }
}
void
ESGE_ObjMouseEvent::MouseWheel(void)
{
  for (ESGE_ObjMouseEvent *obj = list; obj != NULL; obj = obj->next)
  {
    obj->OnMouseWheel(
      ESGE_event.wheel.mouseX,
      ESGE_event.wheel.mouseY,
      ESGE_event.wheel.x,
      ESGE_event.wheel.y
    );
  }
}


void
ESGE_ObjMouseEvent::OnMouseMove(
  SDL_UNUSED int x,
  SDL_UNUSED int y,
  SDL_UNUSED int relX,
  SDL_UNUSED int relY
)
{}
void
ESGE_ObjMouseEvent::OnMouseButtonDown(
  SDL_UNUSED int x,
  SDL_UNUSED int y,
  SDL_UNUSED Uint8 button
)
{}
void
ESGE_ObjMouseEvent::OnMouseButtonUp(
  SDL_UNUSED int x,
  SDL_UNUSED int y,
  SDL_UNUSED Uint8 button
)
{}
void
ESGE_ObjMouseEvent::OnMouseWheel(
  SDL_UNUSED int x,
  SDL_UNUSED int y,
  SDL_UNUSED int wheelX,
  SDL_UNUSED int wheelY
)
{}



ESGE_ObjJoyEvent *ESGE_ObjJoyEvent::list = NULL;


ESGE_ObjJoyEvent::ESGE_ObjJoyEvent(void)  {}
ESGE_ObjJoyEvent::~ESGE_ObjJoyEvent(void) {}


void
ESGE_ObjJoyEvent::EnableJoyEvent(void)
{
  enabledJoyEvent = true;
  SGLIB_LIST_ADD(ESGE_ObjJoyEvent, list, this, next);
}

void
ESGE_ObjJoyEvent::DisableJoyEvent(void)
{
  enabledJoyEvent = false;
  SGLIB_LIST_DELETE(ESGE_ObjJoyEvent, list, this, next);
}

bool
ESGE_ObjJoyEvent::IsEnabledJoyEvent(void)
{
  return enabledJoyEvent;
}


void
ESGE_ObjJoyEvent::JoyAxis(void)
{
  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jaxis.which)
      obj->OnJoyAxis(ESGE_event.jaxis.axis, ESGE_event.jaxis.value);
  }
}
void
ESGE_ObjJoyEvent::JoyBall(void)
{
  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jball.which)
    {
      obj->OnJoyBall(
        ESGE_event.jball.ball,
        ESGE_event.jball.xrel,
        ESGE_event.jball.yrel
      );
    }
  }
}
void
ESGE_ObjJoyEvent::JoyHat(void)
{
  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jhat.which)
      obj->OnJoyHat(ESGE_event.jhat.hat, ESGE_event.jhat.value);
  }
}
void
ESGE_ObjJoyEvent::JoyButtonDown(void)
{
  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jbutton.which)
      obj->OnJoyButtonDown(ESGE_event.jbutton.button);
  }
}
void
ESGE_ObjJoyEvent::JoyButtonUp(void)
{
  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jbutton.which)
      obj->OnJoyButtonUp(ESGE_event.jbutton.button);
  }
}
void
ESGE_ObjJoyEvent::JoyDeviceAdd(void)
{
  if (!SDL_JoystickOpen(ESGE_event.jdevice.which))
  {
    ESGE_Error(
      "Cannot open joystick with id %d",
      ESGE_event.jdevice.which
    );
  }

  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jdevice.which)
      obj->OnJoyDeviceAdd(ESGE_event.jdevice.which);
  }
}
void
ESGE_ObjJoyEvent::JoyDeviceRem(void)
{
  SDL_Joystick *joy;

  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jdevice.which)
      obj->OnJoyDeviceRem(ESGE_event.jdevice.which);
  }

  joy = SDL_JoystickFromInstanceID(ESGE_event.jdevice.which);
  SDL_assert(joy);
  SDL_JoystickClose(joy);
}
void
ESGE_ObjJoyEvent::JoyBattery(void)
{
  for (ESGE_ObjJoyEvent *obj = list; obj != NULL; obj = obj->next)
  {
    if (obj->joyID == -1 || obj->joyID == ESGE_event.jbattery.which)
      obj->OnJoyBattery(ESGE_event.jbattery.level);
  }
}


void
ESGE_ObjJoyEvent::OnJoyAxis(
  SDL_UNUSED Uint8 axis,
  SDL_UNUSED Sint16 value
)
{}
void
ESGE_ObjJoyEvent::OnJoyBall(
  SDL_UNUSED Uint8 ball,
  SDL_UNUSED Sint16 relX,
  SDL_UNUSED Sint16 relY
)
{}
void
ESGE_ObjJoyEvent::OnJoyHat(
  SDL_UNUSED Uint8 hat,
  SDL_UNUSED Uint8 value
)
{}
void
ESGE_ObjJoyEvent::OnJoyButtonDown(SDL_UNUSED Uint8 button)
{}
void
ESGE_ObjJoyEvent::OnJoyButtonUp(SDL_UNUSED Uint8 button)
{}
void
ESGE_ObjJoyEvent::OnJoyDeviceAdd(SDL_UNUSED SDL_JoystickID joyID)
{}
void
ESGE_ObjJoyEvent::OnJoyDeviceRem(SDL_UNUSED SDL_JoystickID joyID)
{}
void
ESGE_ObjJoyEvent::OnJoyBattery(
  SDL_UNUSED SDL_JoystickPowerLevel level
)
{}



SDL_Event ESGE_event;

bool ESGE_quit = false;

void
ESGE_EventLoop(void)
{
  while (SDL_PollEvent(&ESGE_event))
  {
    ESGE_ObjEvent::Event();

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
    case SDL_MOUSEMOTION:
      ESGE_ObjMouseEvent::MouseMove();
      break;
    case SDL_MOUSEBUTTONDOWN:
      ESGE_ObjMouseEvent::MouseButtonDown();
      break;
    case SDL_MOUSEBUTTONUP:
      ESGE_ObjMouseEvent::MouseButtonUp();
      break;
    case SDL_MOUSEWHEEL:
      ESGE_ObjMouseEvent::MouseWheel();
      break;
    case SDL_JOYAXISMOTION:
      ESGE_ObjJoyEvent::JoyAxis();
      break;
    case SDL_JOYBALLMOTION:
      ESGE_ObjJoyEvent::JoyBall();
      break;
    case SDL_JOYHATMOTION:
      ESGE_ObjJoyEvent::JoyHat();
      break;
    case SDL_JOYBUTTONDOWN:
      ESGE_ObjJoyEvent::JoyButtonDown();
      break;
    case SDL_JOYBUTTONUP:
      ESGE_ObjJoyEvent::JoyButtonUp();
      break;
    case SDL_JOYDEVICEADDED:
      ESGE_ObjJoyEvent::JoyDeviceAdd();
      break;
    case SDL_JOYDEVICEREMOVED:
      ESGE_ObjJoyEvent::JoyDeviceRem();
      break;
    case SDL_JOYBATTERYUPDATED:
      ESGE_ObjJoyEvent::JoyBattery();
      break;
    default:
      break;
    }
  }
}
