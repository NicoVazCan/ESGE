#ifndef ESGE_OBJ_SCENE_EDITOR_H_
# define ESGE_OBJ_SCENE_EDITOR_H_

# include <SDL2/SDL.h>
# include "ESGE_game.h"
# include "ESGE_objSerial.h"
# include "ESGE_objScene.h"
# include "ESGE_objEvent.h"

# include <stdio.h>

class ESGE_ObjSceneEditor: public ESGE_ObjQuitEvent
{
  char file[ESGE_OBJ_SCENE_FILE_LEN];
  ESGE_ObjScene *scene;

public:
  ESGE_ObjSceneEditor(const char *file);
  ~ESGE_ObjSceneEditor(void);
  void RunCMD(void);

protected:
  virtual void OnQuit(void);
};

typedef ESGE_ObjSerial *(*ESGE_TextLoad)(void);

class ESGE_TextLoader
{
  static ESGE_TextLoader *list;
  ESGE_TextLoader *next;
  const Uint16 typeID;
  const ESGE_TextLoad load;
  
public:
  ESGE_TextLoader(Uint16 typeID, ESGE_TextLoad load);
  ~ESGE_TextLoader(void);
  static ESGE_ObjSerial *Load(Uint16 typeID);
};

#endif