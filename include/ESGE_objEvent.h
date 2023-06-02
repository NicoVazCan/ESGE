#ifndef ESGE_OBJ_EVENT_H_
# define ESGE_OBJ_EVENT_H_

# include <SDL2/SDL.h>


class ESGE_ObjQuitEvent
{
  friend void ESGE_OnQuit(
    void *userdata,
    SDL_UNUSED SDL_Event *pEvent
  );
protected:
  ESGE_ObjQuitEvent();
  virtual ~ESGE_ObjQuitEvent();
  virtual void OnQuit(void) = 0;
};

class ESGE_ObjKeyEvent
{
  friend void ESGE_OnKey(void *userdata, SDL_Event *pEvent);
protected:
  Uint32 windowID;
  ESGE_ObjKeyEvent();
  virtual ~ESGE_ObjKeyEvent();
  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod) = 0;
  virtual void OnKeyUp(SDL_Keycode key, SDL_Keymod mod) = 0;
};

#endif
