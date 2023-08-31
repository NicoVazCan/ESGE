/**
 * \file ESGE_scene.h
 * \brief Contains all the necessary classes for the handling of scenes and their game objects.
 */
#ifndef ESGE_SCENE_H_
# define ESGE_SCENE_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"
# include "ESGE_objSerial.h"
# include "ESGE_objActive.h"


/**
 * Scene game object instance name maximum length including '\0'. 
 */
# define ESGE_INST_NAME_LEN 32

/**
 * Game object base abstract class which must inherit all game object
 * classes whose instances belong to scenes.
 */
class ESGE_ObjScene:
  public ESGE_ObjSerial,
  public ESGE_ObjActive
{
public:
  /**
   * Use this function to create a new game object instance in the
   * current active scene.
   * The new instance will be named with its class name with a
   * numerical suffix.
   * The new instance will be enabled just before the drawing pipeline
   * of the current frame, so it will not be updated until the next
   * frame.
   * If the game object class was not passed to ESGE_TYPE or
   * ESGE_TYPE_FIELDS, the program will exit showing this error.
   * 
   * \param typeName the name of the game object class to be
   *        instantiated.
   * \return the new instance. Cast to concrete class if needed.
   * 
   * \sa ESGE_ObjScene::Destroy
   */
  static ESGE_ObjScene *Create(const char *typeName);

  /**
   * Internal use only, do not modify.
   * Enum to identify what to do with the instance before drawing
   * pipeline based on the function calls ESGE_ObjScene::Create and
   * ESGE_ObjScene::Destroy.
   */
  enum {OK, ADD, DEL} state;
  /**
   * Read only.
   * Identifier of the scene to which the game object instance
   * belongs.
   */
  Uint64 sceneID;
  /**
   * Read only.
   * Name of the game object instance.
   */
  char instName[ESGE_INST_NAME_LEN];
  /**
   * Read only.
   * Identifier of the game object instance.
   */
  Uint64 instID;
  /**
   * \var ESGE_ObjScene *ESGE_ObjScene::next
   * Internal use only, do not modify.
   * Next game object instance in the scene which belongs.
   */
  /**
   * \var ESGE_ObjScene *ESGE_ObjScene::nextShared
   * Internal use only, do not modify.
   * Next game object instance of the same class which was shared.
   */
  ESGE_ObjScene *next, *nextShared;

  /**
   * Constructor to be called from the derived classes.
   */
  ESGE_ObjScene(void);
  /**
   * Destructor to be called from the derived classes.
   */
  virtual ~ESGE_ObjScene(void) = 0;

  /**
   * Use this function to remove this game object instance from the
   * scene which belongs and then destroy it.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline so it is safe to continue using the instance
   * after calling the function.
   * 
   * \sa ESGE_ObjScene::Create
   */
  void Destroy(void);

# ifdef ESGE_EDITOR
  /**
   * Do not use this function.
   * Callback to enable the components you want to use during editor
   * execution.
   * Declare and define only when compiling the editor using the
   * preprocessor to check the macro ESGE_EDITOR.
   * 
   * \sa ESGE_ObjScene::OnEditorDisable
   */
  virtual void OnEditorEnable(void);
  /**
   * Do not use this function.
   * Callback to disable the components you want to use during editor
   * execution.
   * Declare and define only when compiling the editor using the
   * preprocessor to check the macro ESGE_EDITOR.
   * 
   * \sa ESGE_ObjScene::OnEditorEnable
   */
  virtual void OnEditorDisable(void);
# endif

  /**
   * Do not use this function.
   * Callback called just after all game objects instances belonging
   * to a scene have been created and added.
   * This is called just once during game object instance life.
   * 
   * \sa ESGE_ObjScene::OnQuit
   */
  virtual void OnInit(void);
  /**
   * Do not use this function.
   * Callback called just before all game objects instances belonging
   * to a scene will be destroyed and removed.
   * This is called just once during game object instance life.
   * 
   * \sa ESGE_ObjScene::OnInit
   */
  virtual void OnQuit(void);

  /**
   * Do not use this function.
   * Callback called just after all game objects instances belonging
   * to a scene have been enabled.
   * 
   * \sa ESGE_ObjSerial::OnEnable
   */
  virtual void OnStart(void);
};


/**
 * Use this function to get the shared game object instance list of
 * class \a C.
 * 
 * \tparam C a game object class derived from ESGE_ObjScene.
 * \return a reference to a pointer of the first instance as a list.
 * 
 * \sa ESGE_ObjScene::nextShared
 * \sa ESGE_ShareObj
 * \sa ESGE_UnhareObj
 */
template<class C>
ESGE_ObjScene*&
ESGE_GetSharedList(void)
{
  static ESGE_ObjScene *sharedList = NULL;

  return sharedList;
}

/**
 * Use this function to shared a game object instance \a obj of class
 * \a C.
 * 
 * \tparam C a game object class derived from ESGE_ObjScene.
 * \param obj a game instance to be shared. Must not be shared
 *        already.
 * 
 * \sa ESGE_UnshareObj
 */
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

/**
 * Use this function to unshared a game object instance \a obj of
 * class \a C.
 * 
 * \tparam C a game object class derived from ESGE_ObjScene.
 * \param obj a game instance to be unshared. Must have been shared
 *        before.
 * 
 * \sa ESGE_ShareObj
 */
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

/**
 * Use this function to get the shared game object instance of
 * class \a C named as \a instName from scene loaded from \a
 * sceneFile.
 * 
 * \tparam C a game object class derived from ESGE_ObjScene.
 * \param sceneFile the file of the scene to search for the instance.
 * \param instName the instance name to search.
 * \return the matching instance or NULL if not found.
 * 
 * \sa ESGE_ShareObj
 * \sa ESGE_UnhareObj
 */
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

/**
 * Use this function to get the shared game object instance of
 * class \a C named as \a instName from scene with the id \a sceneID.
 * 
 * \tparam C a game object class derived from ESGE_ObjScene.
 * \param sceneID the id of the scene to search for the instance.
 * \param instName the instance name to search.
 * \return the matching instance or NULL if not found.
 * 
 * \sa ESGE_ShareObj
 * \sa ESGE_UnhareObj
 */
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

/**
 * Use this function to get the first shared game object instance of
 * class \a C named as \a instName from any scene.
 * 
 * \tparam C a game object class derived from ESGE_ObjScene.
 * \param instName the instance name to search.
 * \return the matching instance or NULL if not found.
 * 
 * \sa ESGE_ShareObj
 * \sa ESGE_UnhareObj
 */
template<class C>
C*
ESGE_GetObj(const char *instName)
{
  ESGE_ObjScene *sharedObj;
  Uint64 instID;

  instID = ESGE_Hash(instName);

  for (
    sharedObj = ESGE_GetSharedList<C>();
    sharedObj && sharedObj->instID != instID;
    sharedObj = sharedObj->nextShared
  );

  return (
    sharedObj && sharedObj->instID == instID ? (C*)sharedObj : NULL
  );
}


/**
 * Class that represents a game scene that is nothing more than a
 * collection of game object instances.
 */
class ESGE_Scene
{
public:
  /**
   * Internal use only, do not modify.
   * Enum to identify what to do with the scene before drawing
   * pipeline based on the function calls ESGE_SceneMngr::AddScene,
   * ESGE_SceneMngr::ChangeScene, ESGE_SceneMngr::StashScene, y
   * ESGE_SceneMngr::CloseScene.
   */
  enum {OK, EN, DIS, CLO} state;
  /**
   * Read only.
   * File name from which the scene was loaded.
   */
  const char *sceneFile;
  /**
   * Read only.
   * Scene identifier.
   */
  const Uint64 id;
  /**
   * Read only.
   * Sorted list of game object instances.
   */
  ESGE_ObjScene *objList = NULL;
  /**
   * Internal use only, do not modify.
   * Next scene in a list.
   */
  ESGE_Scene *next;
  
  /**
   * Internal use only.
   * Constructor to load a new scene.
   * 
   * \param sceneFile file from which the new scene will be loaded.
   *        If the file does not exists, an empty scene will be
   *        created.
   */
  ESGE_Scene(const char *sceneFile);
  /**
   * Internal use only.
   * Destructor to close the scene, destroying all its game object
   * instances and then itself.
   */
  ~ESGE_Scene(void);

  /**
   * Internal use only.
   * This function initializes all the scene game object instances.
   * Used by ESGE_SceneMngr.
   */
  void Init(void);
  /**
   * Internal use only.
   * This function finalizes all the scene game object instances.
   * Used by ESGE_SceneMngr.
   */
  void Quit(void);

  /**
   * Internal use only.
   * This function enables all the scene game object instances.
   * Used by ESGE_SceneMngr.
   */
  void Enable(void);
  /**
   * Internal use only.
   * This function disables all the scene game object instances.
   * Used by ESGE_SceneMngr.
   */
  void Disable(void);

  /**
   * Internal use only.
   * This function saves all instances of scene set objects in the
   * file from which they were loaded.
   * Used by ESGE_SceneMngr.
   */
  int Save(void);

  /**
   * Internal use only.
   * This function creates a new game object instance to be added in
   * the scene.
   * The new instance will be named with its class name with a
   * numerical suffix.
   * The new instance will be enabled just before the drawing pipeline
   * of the current frame, so it will not be updated until the next
   * frame.
   * Used by the scene editor program tool and ESGE_ObjScene::Create.
   * 
   * \param typeName the name of the game object class to be
   *        instantiated.
   * \return the new instance or NULL if the game object class was not
   *         passed to ESGE_TYPE or ESGE_TYPE_FIELDS. Check
   *         SDL_GetError for get more info about error.
   */
  ESGE_ObjScene *AddObj(const char *typeName);
  /**
   * Internal use only.
   * This function destroys a removes game object instance from the
   * scene.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline so it is safe to continue using the instance
   * after calling the function.
   * Used by the scene editor program tool and ESGE_ObjScene::Destroy.
   * 
   * \param instName the name of the game object class to be
   *        destroyed and removed.
   */
  void DelObj(const char *instName);
  /**
   * Internal use only.
   * This function performs addition and deletion of the game objects
   * instances to the scene if requested by ESGE_Scene::AddObj and
   * ESGE_Scene::DelObj respectively.
   * Called in the main loop just before drawing pipeline.
   */
  void Update(void);

  /**
   * Internal use only.
   * This function return the game object instance named as
   * \a instName from the scene if exist.
   * Used by the scene editor program tool.
   * 
   * \param instName the name of the instance.
   * 
   * \return the instance or NULL if not found. Check SDL_GetError for
   *         get more info about error.
   */
  ESGE_ObjScene *GetObj(const char *instName);
# ifdef ESGE_EDITOR
  /**
   * Internal use only.
   * This function renames the game object instance name as \a
   * instName to \a newInstName.
   * Used by the scene editor program tool.
   * 
   * \param instName the name of the instance.
   * \param newInstName the new name of the instance.
   * 
   * \return 0 or -1 if the name \a newInstName was already used.
   *         Check SDL_GetError for get more info about error.
   */
  int RenameObj(
    const char *instName,
    const char *newInstName
  );
# endif
};

/**
 * Singleton class to manage the loading and unloading of scenes.
 */
class ESGE_SceneMngr
{
  /**
   * \var ESGE_Scene *ESGE_SceneMngr::active
   * The current active scene in which the new game object
   * instances will be added using ESGE_Scene::AddObj and
   * ESGE_ObjScene::Create.
   */
  /**
   * \var ESGE_Scene *ESGE_SceneMngr::enabledList
   * A stack containing the current enabled scenes.
   */
  /**
   * \var ESGE_Scene *ESGE_SceneMngr::lastDisabled
   * A stack containing the current disabled scenes.
   * Its size will be less or equal to ESGE_SceneMngr::maxDisabled.
   */
  static ESGE_Scene
    *active,
    *enabledList,
    *lastDisabled;
  /**
   * \var ESGE_Scene *ESGE_SceneMngr::maxDisabled
   * The max number of disabled scenes.
   */
  static int maxDisabled;

  /**
   * Not used (singleton).
   */
  ESGE_SceneMngr(void);
  /**
   * Not used (singleton).
   */
  ~ESGE_SceneMngr(void);

public:
  /**
   * Internal use only.
   * This function sets the max number of disabled scenes.
   * It is called before main loop.
   */
  static void Init(int maxDisabled);
  /**
   * Internal use only.
   * This function disables, finalizes, and destroy all enabled scenes
   * and then finalizes, and destroy all disabled scenes.
   * It is called after main loop.
   */
  static void Quit(void);

  /**
   * Internal use only.
   * This function performs addition, stashing, and closing of the
   * scenes if requested by ESGE_SceneMngr::AddScene,
   * ESGE_SceneMngr::StashScene, and ESGE_SceneMngr::CloseScene
   * respectively.
   * Called in the main loop just before drawing pipeline.
   */
  static void Update(void);

  /**
   * Use this function to load a new scene from the file \a sceneFile
   * keeping the previous loaded scenes enabled.
   * If the scene was already loaded but stashed and remains in the
   * disabled scenes stack ESGE_SceneMngr::lastDisabled, it will be
   * enabled back.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline.
   * The scene will be set as the active one immediately.
   */
  static void AddScene(const char *sceneFile);

  /**
   * Use this function to load a new scene from the file \a sceneFile
   * stashing all enabled scenes.
   * If the scene was already loaded but stashed and remains in the
   * disabled scenes stack ESGE_SceneMngr::lastDisabled, it will be
   * enabled back.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline.
   * The scene will be set as the active one immediately.
   * 
   * \param sceneFile file from which the scene was be loaded.
   */
  static void ChangeScene(const char *sceneFile);

  /**
   * Use this function to stash the scene loaded from the file \a
   * sceneFile.
   * The scene if is enabled it will be disabled, and nothing
   * otherwise.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline.
   * The next enabled scene will be set as the active one immediately.
   * 
   * \param sceneFile file from which the scene was be loaded.
   */
  static void StashScene(const char *sceneFile);
  /**
   * Use this function to stash the scene with \a id id.
   * The scene if is enabled it will be disabled, and nothing
   * otherwise.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline.
   * The next enabled scene will be set as the active one immediately.
   * 
   * \param id the scene identifier.
   */
  static void StashScene(Uint64 id);

  /**
   * Use this function to close the scene loaded from the file \a
   * sceneFile.
   * The scene if is enabled it will be disabled and destroyed, and
   * only destroyed otherwise.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline.
   * If the scene was enabled, the next scene will be set as the
   * active one immediately.
   * 
   * \param sceneFile file from which the scene was be loaded.
   */
  static void CloseScene(const char *sceneFile);
  /**
   * Use this function to close the scene with \a id id.
   * The scene if is enabled it will be disabled and destroyed, and
   * only destroyed otherwise.
   * This will not be immediate, it is postponed to just before the
   * drawing pipeline.
   * If the scene was enabled, the next scene will be set as the
   * active one immediately.
   * 
   * \param id the scene identifier.
   */
  static void CloseScene(Uint64 id);

  /**
   * Use this function to get the active scene.
   * 
   * \return the active scene
   */
  static ESGE_Scene *GetActiveScene(void);
  /**
   * Use this function to set the active scene.
   *  
   * \param sceneFile the file from which the scene was be loaded.
   */
  static int SetActiveScene(const char *sceneFile);
  /**
   * Use this function to set the active scene.
   *  
   * \param id the scene id.
   */
  static int SetActiveScene(Uint64 id);
};

#endif