#ifndef ESGE_OBJ_EVENT_H_
# define ESGE_OBJ_EVENT_H_

# include <SDL2/SDL.h>
# include "ESGE_event.h"
# include "ESGE_objActive.h"

class ESGE_ObjQuitEvent: public virtual ESGE_ObjActive
{
public:
  ESGE_ObjQuitEvent(void);
  virtual ~ESGE_ObjQuitEvent(void) = 0;

  virtual void OnEnable(void);
  virtual void OnDisable(void);

  virtual void OnQuit(void);
};

class ESGE_ObjKeyEvent: public virtual ESGE_ObjActive
{
public:
  Uint32 windowID = 0;
  ESGE_ObjKeyEvent(void);
  virtual ~ESGE_ObjKeyEvent(void) = 0;

  virtual void OnEnable(void);
  virtual void OnDisable(void);

  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod);
  virtual void OnKeyUp(SDL_Keycode key, SDL_Keymod mod);
};

#endif
