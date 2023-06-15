#ifndef ESGE_OBJ_TILE_H_
# define ESGE_OBJ_TILE_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"
# include "ESGE_objScene.h"
# include "ESGE_objSceneEditor.h"
# include "ESGE_objPoint.h"
# include "ESGE_objCollider.h"
# include "ESGE_objStatic.h"
# include "ESGE_objDraw.h"

# include "ESGE_objCam.h"
# include "ESGE_objDisplay.h"

# define ESGE_OBJ_TILE_TYPE_ID 5
# define ESGE_OBJ_TILE_LAYER 1

class ESGE_ObjTile:
  public ESGE_ObjInScene,
  public ESGE_ObjStatic,
  public ESGE_ObjDraw
{
  const Uint16 displayID, camID;
  ESGE_ObjCam *cam;

public:
  static ESGE_ObjTile *list;
  ESGE_ObjTile *next;

  ESGE_ObjTile(
    Uint16 id,
    SDL_Point pos,
    Uint16 displayID,
    Uint16 camID
  );
  virtual ~ESGE_ObjTile(void) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;

  virtual void OnCollide(ESGE_ObjCollider *other) override;

  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;

protected:
  virtual void OnDraw(SDL_Renderer *rend) override;
};

#endif