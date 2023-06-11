#ifndef ESGE_OBJ_CAM_H_
# define ESGE_OBJ_CAM_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"
# include "ESGE_objScene.h"

# define ESGE_OBJ_CAM_TYPE_ID 16u

class ESGE_ObjCam: public ESGE_ObjInScene
{
  friend ESGE_ObjSerial *ESGE_LoadObjCam(SDL_RWops *io);

public:
  SDL_Rect view;
  static ESGE_ObjCam *list;
  ESGE_ObjCam *next;

  ESGE_ObjCam(Uint16 id, SDL_Rect view);
  virtual ~ESGE_ObjCam(void) override;
  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
};

#endif