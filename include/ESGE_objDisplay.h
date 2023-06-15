#ifndef ESGE_OBJ_DISPLAY_H_
# define ESGE_OBJ_DISPLAY_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"
# include "ESGE_objEvent.h"
# include "ESGE_objScene.h"
# include "ESGE_objSceneEditor.h"

# include <stdio.h>

# define ESGE_OBJ_DISPLAY_TYPE_ID 1

class ESGE_ObjDisplay: public ESGE_ObjKeyEvent, public ESGE_ObjInScene
{
  Uint8 vsync;
public:
  static ESGE_ObjDisplay *list;
  ESGE_ObjDisplay *next;
  SDL_Renderer *rend;

  ESGE_ObjDisplay(Uint16 id, Uint8 full, Uint8 vsync);
  virtual ~ESGE_ObjDisplay(void) override;
  void OnSave(SDL_RWops *io) const;
  Uint16 GetTypeID(void) const;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
protected:
  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod) override;
};

#endif