#ifndef ESGE_OBJ_DISPLAY_H_
# define ESGE_OBJ_DISPLAY_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"
# include "ESGE_objEvent.h"
# include "ESGE_objScene.h"

# define ESGE_OBJ_DISPLAY_TYPE_ID 2

class ESGE_ObjDisplay: public ESGE_ObjKeyEvent, public ESGE_ObjInScene
{
  friend ESGE_ObjSerial *ESGE_LoadObjDisplay(SDL_RWops *io);
  Uint8 vsync;
public:
  static ESGE_ObjDisplay *list;
  ESGE_ObjDisplay *next;
  SDL_Renderer *rend;

  ESGE_ObjDisplay(Uint16 id, Uint8 full, Uint8 vsync);
  ~ESGE_ObjDisplay(void);
  void OnSave(SDL_RWops *io) const;
  Uint16 GetTypeID(void) const;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
protected:
  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod) override;
};

#endif