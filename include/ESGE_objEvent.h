#ifndef ESGE_OBJ_EVENT_H_
# define ESGE_OBJ_EVENT_H_

# include <SDL2/SDL.h>
# include "ESGE_objActive.h"

class ESGE_ObjQuitEvent: public virtual ESGE_ObjActive
{
  friend void ESGE_OnQuit(
    void *userdata,
    SDL_UNUSED SDL_Event *pEvent
  );
public:
  ESGE_ObjQuitEvent(void);
  virtual ~ESGE_ObjQuitEvent(void) = 0;
  virtual void OnEnable(void);
  virtual void OnDisable(void);
protected:
  virtual void OnQuit(void);
};

class ESGE_ObjKeyEvent: public virtual ESGE_ObjActive
{
  friend void ESGE_OnKey(void *userdata, SDL_Event *pEvent);
public:
  ESGE_ObjKeyEvent();
  ESGE_ObjKeyEvent(Uint32 windowID);
  virtual ~ESGE_ObjKeyEvent(void) = 0;
  virtual void OnEnable(void);
  virtual void OnDisable(void);
protected:
  Uint32 windowID;
  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod);
  virtual void OnKeyUp(SDL_Keycode key, SDL_Keymod mod);
};

#endif
