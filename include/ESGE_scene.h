#ifndef ESGE_SCENE_H_
# define ESGE_SCENE_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"
# include "ESGE_objSerial.h"
# include "ESGE_objActive.h"

# define ESGE_INST_NAME_LEN 32


class ESGE_ObjScene:
  public ESGE_ObjSerial,
  public ESGE_ObjActive
{
public:
  static ESGE_ObjScene *Create(const char *typeName);

  enum {OK, ADD, DEL} state;
  Uint64 sceneID;
  char instName[ESGE_INST_NAME_LEN];
  Uint64 instID;
  ESGE_ObjScene *next, *nextShared;

  ESGE_ObjScene(void);
  virtual ~ESGE_ObjScene(void) = 0;

  void Destroy(void);

# ifdef ESGE_EDITOR
  virtual void OnEditorInit(void);
  virtual void OnEditorQuit(void);
# endif

  virtual void OnInit(void);
  virtual void OnQuit(void);

  virtual void OnStart(void);
};


template<class C>
ESGE_ObjScene*&
ESGE_GetSharedList(void)
{
  static ESGE_ObjScene *sharedList = NULL;

  return sharedList;
}

template<class C>
void
ESGE_ShareObj(C *obj)
{
  ESGE_ObjScene *sharedObj, **node;

  sharedObj = obj;

  for (
    node = &ESGE_GetSharedList<C>();
    (
      *node &&
      (*node)->sceneID <= sharedObj->sceneID &&
      (*node)->instID < sharedObj->instID
    );
    node = &(*node)->nextShared
  );

  SDL_assert(
    !*node ||
    (*node)->sceneID != sharedObj->sceneID ||
    (*node)->instID != sharedObj->instID
  );

  sharedObj->nextShared = *node;
  *node = sharedObj;
}

template<class C>
void
ESGE_UnshareObj(C *obj)
{
  ESGE_ObjScene *sharedObj, **node;

  sharedObj = obj;

  for (
    node = &ESGE_GetSharedList<C>();
    *node && *node != sharedObj;
    node = &(*node)->nextShared
  );

  SDL_assert(*node && *node == sharedObj);

  *node = sharedObj->nextShared;
}

template<class C>
C*
ESGE_GetObj(const char *sceneFile, const char *instName)
{
  ESGE_ObjScene *sharedObj;
  Uint64 sceneID, instID;

  sceneID = ESGE_Hash(sceneFile);
  instID = ESGE_Hash(instName);

  for (
    sharedObj = ESGE_GetSharedList<C>();
    (
      sharedObj &&
      sharedObj->sceneID <= sceneID &&
      sharedObj->instID < instID
    );
    sharedObj = sharedObj->nextShared
  );

  return (
    (
      sharedObj &&
      sharedObj->sceneID == sceneID &&
      sharedObj->instID == instID
    ) ?
    (C*)sharedObj :
    NULL
  );
}


template<class C>
C*
ESGE_GetObj(Uint64 sceneID, const char *instName)
{
  ESGE_ObjScene *sharedObj;
  Uint64 instID;

  instID = ESGE_Hash(instName);

  for (
    sharedObj = ESGE_GetSharedList<C>();
    (
      sharedObj &&
      sharedObj->sceneID <= sceneID &&
      sharedObj->instID < instID
    );
    sharedObj = sharedObj->nextShared
  );

  return (
    (
      sharedObj &&
      sharedObj->sceneID == sceneID &&
      sharedObj->instID == instID
    ) ?
    (C*)sharedObj :
    NULL
  );
}


class ESGE_Scene
{
public:
  enum {OK, EN, DIS, CLO} state;
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
  void Update(void);

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
    *lastDisabled;

  ESGE_SceneMngr(void);
  ~ESGE_SceneMngr(void);

public:
  static int maxDisabled;

  static void Init(int maxDisabled);
  static void Quit(void);

  static void Update(void);

  static void AddScene(const char *sceneFile);

  static void ChangeScene(const char *sceneFile);

  static void StashScene(const char *sceneFile);
  static void StashScene(Uint64 id);

  static void CloseScene(const char *sceneFile);
  static void CloseScene(Uint64 id);

  static ESGE_Scene *GetActiveScene(void);
  static int SetActiveScene(const char *sceneFile);
  static int SetActiveScene(Uint64 id);
};

#endif