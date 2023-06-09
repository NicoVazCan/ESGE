#ifndef ESGE_OBJ_SCENE_H_
# define ESGE_OBJ_SCENE_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"

# define ESGE_OBJ_SCENE_TYPE_ID 1u

class ESGE_ObjInScene: public ESGE_ObjSerial
{
public:
  static ESGE_ObjInScene *list;
  ESGE_ObjInScene *next;

  ESGE_ObjInScene(Uint16 id);
  ~ESGE_ObjInScene(void);
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
};

class ESGE_ObjScene: ESGE_ObjSerial
{
  ESGE_ObjInScene *disList = NULL;
public:
  friend ESGE_ObjSerial *ESGE_LoadObjScene(SDL_RWops *io);
  ESGE_ObjScene(Uint16 id, ESGE_ObjInScene *disList);
  virtual ~ESGE_ObjScene(void) override;
  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
};

#endif