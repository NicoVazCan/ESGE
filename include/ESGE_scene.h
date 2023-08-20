#ifndef ESGE_SCENE_H_
# define ESGE_SCENE_H_

# include <SDL2/SDL.h>
# include "ESGE_file.h"
# include "ESGE_objSerial.h"
# include "ESGE_objActive.h"

# define ESGE_INST_NAME_LEN 32

class ESGE_ObjScene:
  public ESGE_ObjSerial,
  public virtual ESGE_ObjActive
{
public:
  static inline Uint64 Cmp(
    const ESGE_ObjScene *left,
    const ESGE_ObjScene *right
  )
  {
    Uint64 diff;

    if ((diff = left->sceneID - right->sceneID)) return diff;
    return      left->instID  - right->instID;
  }

  Uint64 sceneID;
  char instName[ESGE_INST_NAME_LEN];
  Uint64 instID;
  ESGE_ObjScene *next;

  ESGE_ObjScene(void);
  virtual ~ESGE_ObjScene(void) = 0;

# ifdef ESGE_EDITOR
  virtual void OnEditorInit(void);
  virtual void OnEditorQuit(void);
# endif

  virtual void OnInit(void);
  virtual void OnQuit(void);
};

class ESGE_Scene
{
public:
  const char *sceneFile;
  const Uint64 id;
  ESGE_ObjScene *objList = NULL;
  ESGE_Scene *next;
  
  ESGE_Scene(const char *sceneFile);
  ~ESGE_Scene(void);

  void Enable(void);
  void Disable(void);

  int Save(void);

  ESGE_ObjScene *AddObj(const char *typeName);
  void DelObj(const char *instName);
  ESGE_ObjScene *GetObj(const char *instName);
# ifdef ESGE_EDITOR
  int RenameObj(
    const char *instName,
    const char *newInstName
  );
# endif
};



class ESGE_SceneMngr
{
  static ESGE_Scene
    *active,
    *enabledList,
    *disabledList,
    *lastDisabled;
  static int nDisabled;

  static void EnableScene(ESGE_Scene *scene);
  static void DisableScene(ESGE_Scene *scene);

  ESGE_SceneMngr(void);
  ~ESGE_SceneMngr(void);

public:
  static int maxDisabled;

  static void Init(int maxDisabled);
  static void Quit(void);

  static void AddScene(const char *sceneFile);
  static void ChangeScene(const char *sceneFile);

  static void CloseScene(const char *sceneFile);

  static ESGE_Scene *GetActiveScene(void);
};

#endif