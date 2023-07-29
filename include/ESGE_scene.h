#ifndef ESGE_SCENE_H_
# define ESGE_SCENE_H_

# include <SDL2/SDL.h>
# include "ESGE_file.h"
# include "ESGE_hash.h"
# include "ESGE_objSerial.h"
# include "ESGE_objPoint.h"

# define ESGE_INST_NAME_LEN 32

class ESGE_ObjScene: public ESGE_ObjSerial
{
public:
  SDL_FORCE_INLINE Uint64 Cmp(
    const ESGE_ObjScene *left,
    const ESGE_ObjScene *right
  )
  {
     int diff;

    if ((diff = left->sceneID - right->sceneID)) return diff;
    return      left->instID  - right->instID;
  }

  Uint64 sceneID = 0;
  char instName[ESGE_INST_NAME_LEN];
  Uint64 instID = 0;
  ESGE_ObjScene *next;

  ESGE_ObjScene(void);
  virtual ~ESGE_ObjScene(void) = 0;
};

class ESGE_Scene: public ESGE_File
{
public:
  ESGE_ObjScene *objList = NULL;
  
  ESGE_Scene(const char *file);
  virtual ~ESGE_Scene(void) override;

  int Save(void);

  ESGE_ObjScene *AddObj(const char *typeName);
  void DelObj(const char *instName);
  ESGE_ObjScene *GetObj(const char *instName);
  int RenameObj(
    const char *instName,
    const char *newInstName
  );
};

#endif