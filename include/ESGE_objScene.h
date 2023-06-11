#ifndef ESGE_OBJ_SCENE_H_
# define ESGE_OBJ_SCENE_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"

# define ESGE_OBJ_SCENE_FILE_LEN 32

class ESGE_ObjInScene: public ESGE_ObjSerial
{
public:
  ESGE_ObjInScene *next;

  ESGE_ObjInScene(Uint16 id);
  virtual ~ESGE_ObjInScene(void) override;
};

class ESGE_ObjScene
{
  ESGE_ObjInScene *objList = NULL;

public:
  char file[ESGE_OBJ_SCENE_FILE_LEN];
  static ESGE_ObjScene *list;
  ESGE_ObjScene *next;

  ESGE_ObjScene(const char *file);
  virtual ~ESGE_ObjScene(void);
  void Save(void);
  void Enable(void);
  void Disable(void);
  void AddObj(ESGE_ObjInScene *obj);
  void DelObj(ESGE_ObjInScene *obj);
};

#endif