#ifndef ESGE_OBJ_SCENE_H_
# define ESGE_OBJ_SCENE_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"

# define ESGE_OBJ_SCENE_TYPE_ID 1u

class ESGE_ObjInScene: public ESGE_ObjSerial
{
public:
  ESGE_ObjInScene *next;

  ESGE_ObjInScene(Uint16 id);
  virtual ~ESGE_ObjInScene(void) override;
};

class ESGE_ObjScene: public ESGE_ObjInScene
{
  ESGE_ObjInScene *objList;
public:
  friend ESGE_ObjSerial *ESGE_LoadObjScene(SDL_RWops *io);
  ESGE_ObjScene(Uint16 id, ESGE_ObjInScene *objList);
  virtual ~ESGE_ObjScene(void) override;
  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
  void AddObj(ESGE_ObjInScene *obj);
  void DelObj(ESGE_ObjInScene *obj);
};

#endif