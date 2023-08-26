#ifndef ESGE_OBJ_EVENT_H_
# define ESGE_OBJ_EVENT_H_

# include <SDL2/SDL.h>


class ESGE_Event
{
  static ESGE_Event *list;

  ESGE_Event *next = NULL;
  bool enabledEvent = false;

public:
  static void Event(void);

  ESGE_Event(void);
  virtual ~ESGE_Event(void) = 0;

  void EnableEvent(void);
  void DisableEvent(void);
  bool IsEnabledEvent(void);

  virtual void OnEvent(void);
};

class ESGE_ObjKeyEvent
{
  static ESGE_ObjKeyEvent *list;

  ESGE_ObjKeyEvent *next = NULL;
  bool enabledKeyEvent = false;

public:
  static void KeyDown(void);
  static void KeyUp(void);

  ESGE_ObjKeyEvent(void);
  virtual ~ESGE_ObjKeyEvent(void) = 0;

  void EnableKeyEvent(void);
  void DisableKeyEvent(void);
  bool IsEnabledKeyEvent(void);

  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod);
  virtual void OnKeyUp(SDL_Keycode key, SDL_Keymod mod);
};

class ESGE_ObjMouseEvent
{
  static ESGE_ObjMouseEvent *list;

  ESGE_ObjMouseEvent *next = NULL;
  bool enabledMouseEvent = false;

public:
  static void MouseMove(void);
  static void MouseButtonDown(void);
  static void MouseButtonUp(void);
  static void MouseWheel(void);

  ESGE_ObjMouseEvent(void);
  virtual ~ESGE_ObjMouseEvent(void) = 0;

  void EnableMouseEvent(void);
  void DisableMouseEvent(void);
  bool IsEnabledMouseEvent(void);

  virtual void OnMouseMove(int x, int y, int relX, int relY);
  virtual void OnMouseButtonDown(int x, int y, Uint8 button);
  virtual void OnMouseButtonUp(int x, int y, Uint8 button);
  virtual void OnMouseWheel(int x, int y, int wheelX, int wheelY);
};


class ESGE_ObjJoyEvent
{
  static ESGE_ObjJoyEvent *list;

  ESGE_ObjJoyEvent *next = NULL;
  bool enabledJoyEvent = false;

public:
  static void JoyAxis(void);
  static void JoyBall(void);
  static void JoyHat(void);
  static void JoyButtonDown(void);
  static void JoyButtonUp(void);
  static void JoyDeviceAdd(void);
  static void JoyDeviceRem(void);
  static void JoyBattery(void);

  SDL_JoystickID joyID = -1;

  ESGE_ObjJoyEvent(void);
  virtual ~ESGE_ObjJoyEvent(void) = 0;

  void EnableJoyEvent(void);
  void DisableJoyEvent(void);
  bool IsEnabledJoyEvent(void);

  virtual void OnJoyAxis(Uint8 axis, Sint16 value);
  virtual void OnJoyBall(Uint8 ball, Sint16 relX, Sint16 relY);
  virtual void OnJoyHat(Uint8 hat, Uint8 value);
  virtual void OnJoyButtonDown(Uint8 button);
  virtual void OnJoyButtonUp(Uint8 button);
  virtual void OnJoyDeviceAdd(SDL_JoystickID joyID);
  virtual void OnJoyDeviceRem(SDL_JoystickID joyID);
  virtual void OnJoyBattery(SDL_JoystickPowerLevel level);
};



extern SDL_Event ESGE_event;
extern bool ESGE_quit;

void ESGE_EventLoop(void);

#endif
