#ifndef ESGE_OBJ_EVENT_H_
# define ESGE_OBJ_EVENT_H_

# include <SDL2/SDL.h>


class ESGE_Event
{
  ESGE_Event(void);
  ~ESGE_Event(void);

public:
  static SDL_Event event;
  static bool quit;

  static void Loop(void);
};

class ESGE_ObjKeyEvent
{
  static ESGE_ObjKeyEvent *list;

  ESGE_ObjKeyEvent *next = NULL;
  bool enabledKeyEvent = false;

public:
  static void KeyDown(void);
  static void KeyUp(void);

  Uint32 windowID = 0;
  ESGE_ObjKeyEvent(void);
  virtual ~ESGE_ObjKeyEvent(void) = 0;

  void EnableKeyEvent(void);
  void DisableKeyEvent(void);
  bool IsEnabledKeyEvent(void);

  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod);
  virtual void OnKeyUp(SDL_Keycode key, SDL_Keymod mod);
};

#endif
