#ifndef ESGE_OBJ_PLAYER_H_
# define ESGE_OBJ_PLAYER_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"
# include "ESGE_objScene.h"
# include "ESGE_objSceneEditor.h"
# include "ESGE_objPoint.h"
# include "ESGE_objEvent.h"
# include "ESGE_objCollider.h"
# include "ESGE_objDynamic.h"
# include "ESGE_objDraw.h"
# include "ESGE_objUpdate.h"

# include "ESGE_objCam.h"
# include "ESGE_objDisplay.h"
# include "ESGE_objTile.h"

# define ESGE_OBJ_PLAYER_TYPE_ID 4
# define ESGE_OBJ_PLAYER_LAYER 2

class ESGE_ObjPlayer:
  public ESGE_ObjInScene,
  public ESGE_ObjKeyEvent,
  public ESGE_ObjUpdate,
  public ESGE_ObjDynamic,
  public ESGE_ObjDraw
{
  const Uint16 displayID, camID;
  ESGE_ObjCam *cam;

public:
  static ESGE_ObjPlayer* list;
  ESGE_ObjPlayer *next;

  ESGE_ObjPlayer(
    Uint16 id,
    SDL_Point pos,
    Uint16 displayID,
    Uint16 camID
  );
  virtual ~ESGE_ObjPlayer(void) override;
  
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;

  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;

protected:
  virtual void OnUpdate(void) override;

  virtual void OnMove(void) override;
  virtual void OnCollide(ESGE_ObjCollider *other) override;

  virtual void OnDraw(SDL_Renderer *rend) override;
};

#endif