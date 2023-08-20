#include "ESGE_scene.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_hash.h"
#include "ESGE_error.h"
#include "ESGE_io.h"


ESGE_ObjScene::ESGE_ObjScene(void)
{}

ESGE_ObjScene::~ESGE_ObjScene(void)
{}

#ifdef ESGE_EDITOR
void
ESGE_ObjScene::OnEditorInit(void)
{}

void
ESGE_ObjScene::OnEditorQuit(void)
{}
#endif


void
ESGE_ObjScene::OnInit(void)
{}

void
ESGE_ObjScene::OnQuit(void)
{}


ESGE_Scene::ESGE_Scene(const char *sceneFile):
  id(ESGE_Hash(sceneFile))
{
  SDL_RWops *io;

  if(!(this->sceneFile = SDL_strdup(sceneFile)))
    ESGE_Error("Cannot duplicate \"%s\"string", sceneFile);

  if ((io = SDL_RWFromFile(sceneFile, "rb")))
  {
    Sint64 size, tell;

    if ((size = SDL_RWsize(io)) == -1)
    {
      ESGE_Error(
        "Cannot get file \"%s\" size: %s",
        sceneFile,
        SDL_GetError()
      );
    }
    if ((tell = SDL_RWtell(io)) == -1)
      ESGE_Error("Cannot get file \"%s\" position", sceneFile);

    while (tell < size)
    {
      Uint64 typeID;
      const ESGE_Type *type;
      ESGE_ObjScene *obj, **node;

      typeID = ESGE_ReadU64(io);

      if (!(type = ESGE_Type::Get(typeID)))
      {
        ESGE_Error(
          "Cannot find type of typeID=%" SDL_PRIu64 " from "
          "file \"%s\": %s",
          typeID,
          sceneFile,
          SDL_GetError()
        );
      }
      obj = (ESGE_ObjScene*)type->New();

      obj->sceneID = id;
      obj->typeID  = typeID;
      ESGE_ReadStr(io, obj->instName, ESGE_INST_NAME_LEN);
      obj->instID  = ESGE_Hash(obj->instName);

      obj->Load(io);

      for (
        node = &objList;
        *node != NULL && (*node)->instID < obj->instID;
        node = &(*node)->next
      );

      SDL_assert(!*node || (*node)->instID != obj->instID);

      obj->next = *node;
      *node = obj;

      tell = SDL_RWtell(io);
    }
    SDL_RWclose(io);


    for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
    {
#ifdef ESGE_EDITOR
      obj->OnEditorInit();
#else
      obj->OnInit();
#endif
    }
  }
  else SDL_ClearError();
}

ESGE_Scene::~ESGE_Scene(void)
{
  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
  {
#ifdef ESGE_EDITOR
    obj->OnEditorQuit();
#else
    obj->OnQuit();
#endif
  }
  SGLIB_SORTED_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjScene,
    objList,
    obj,
    next,
    {
      delete obj;
    }
  );
  SDL_free((void*)sceneFile);
}


void
ESGE_Scene::Enable(void)
{
  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
    obj->OnEnable();
}

void
ESGE_Scene::Disable(void)
{
  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
    obj->OnDisable();
}

int
ESGE_Scene::Save(void)
{
  SDL_RWops *io;

  if (!(io = SDL_RWFromFile(sceneFile, "wb")))
  {
    return SDL_SetError(
      "Cannot open file while saving \"%s\" scene: %s",
      sceneFile,
      SDL_GetError()
    );
  }

  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
  {
    ESGE_WriteU64(io, obj->typeID);
    ESGE_WriteStr(io, obj->instName);
    obj->Save(io);
  }
  SDL_RWclose(io);

  return 0;
}


ESGE_ObjScene*
ESGE_Scene::AddObj(const char *typeName)
{
  const ESGE_Type *type;
  ESGE_ObjScene *obj, **node;
  Uint64 typeID, instCnt = 0;

  SDL_assert(typeName != NULL);

  typeID = ESGE_Hash(typeName);

  if (!(type = ESGE_Type::Get(typeID)))
  {
    SDL_SetError(
      "Cannot find type \"%s\" in \"%s\" scene",
      typeName,
      sceneFile
    );
    return NULL;
  }
  obj = (ESGE_ObjScene*)type->New();

  obj->sceneID = id;
  obj->typeID  = typeID;
  SDL_strlcpy(obj->instName, typeName, ESGE_INST_NAME_LEN);
  obj->instID = typeID;

  for (
    node = &objList;
    *node != NULL && (*node)->instID < obj->instID;
    node = &(*node)->next
  );

  while (*node && (*node)->instID == obj->instID)
  {
    SDL_snprintf(
      obj->instName,
      ESGE_INST_NAME_LEN,
      "%s%" SDL_PRIu64,
      typeName,
      instCnt++
    );
    obj->instID = ESGE_Hash(obj->instName);

    for (
      node = &objList;
      *node != NULL && (*node)->instID < obj->instID;
      node = &(*node)->next
    );
  }

  obj->next = *node;
  *node = obj;

#ifdef ESGE_EDITOR
  obj->OnEditorInit();
#else
  obj->OnInit();
#endif

  return obj;
}

void
ESGE_Scene::DelObj(const char *instName)
{
  ESGE_ObjScene **node;
  Uint64 instID;

  SDL_assert(instName != NULL);

  instID = ESGE_Hash(instName);

  for (
    node = &objList;
    *node != NULL && (*node)->instID < instID;
    node = &(*node)->next
  );

  if (*node != NULL && (*node)->instID == instID)
  {
    ESGE_ObjScene *next = (*node)->next;

#ifdef ESGE_EDITOR
    (*node)->OnEditorQuit();
#else
    (*node)->OnQuit();
#endif

    delete *node;
    *node = next;
  }
}

ESGE_ObjScene*
ESGE_Scene::GetObj(const char *instName)
{
  ESGE_ObjScene *obj;
  Uint64 instID;

  SDL_assert(instName != NULL);

  instID = ESGE_Hash(instName);

  for (
    obj = objList;
    obj != NULL && obj->instID < instID;
    obj = obj->next
  );

  if (obj != NULL && obj->instID == instID)
  {
    return obj;
  }
  else
  {
    SDL_SetError(
      "Object \"%s\" not found in \"%s\" scene",
      instName,
      sceneFile
    );
    return NULL;
  }
}

#ifdef ESGE_EDITOR
int
ESGE_Scene::RenameObj(const char *instName, const char *newInstName)
{
  ESGE_ObjScene **node;
  Uint64 instID;

  SDL_assert(instName != NULL);

  instID = ESGE_Hash(instName);

  for (
    node = &objList;
    *node != NULL && (*node)->instID < instID;
    node = &(*node)->next
  );

  if (*node != NULL && (*node)->instID == instID)
  {
    ESGE_ObjScene *obj = *node;

    *node = obj->next;
    instID = ESGE_Hash(newInstName);

    for (
      node = &objList;
      *node != NULL && (*node)->instID < instID;
      node = &(*node)->next
    );

    if (*node != NULL && (*node)->instID == instID)
    {
      return SDL_SetError(
        "Object \"%s\" already exist in \"%s\" scene",
        instName,
        sceneFile
      );
    }
    else
    {
      SDL_strlcpy(obj->instName, newInstName, ESGE_INST_NAME_LEN);
      obj->instID = instID;
      obj->next = *node;
      *node = obj;

      return 0;
    }
  }

  return SDL_SetError(
    "Object \"%s\" not found in \"%s\" scene",
    instName,
    sceneFile
  );
}
#endif


ESGE_Scene *ESGE_SceneMngr::active = NULL;
ESGE_Scene *ESGE_SceneMngr::enabledList = NULL;
ESGE_Scene *ESGE_SceneMngr::disabledList = NULL;
ESGE_Scene *ESGE_SceneMngr::lastDisabled = NULL;
int ESGE_SceneMngr::nDisabled = 0;


void
ESGE_SceneMngr::EnableScene(ESGE_Scene *scene)
{
  scene->next = enabledList;
  enabledList = scene;
#ifndef ESGE_EDITOR
  scene->Enable();
#endif
}

void
ESGE_SceneMngr::DisableScene(ESGE_Scene *scene)
{
  scene->next = lastDisabled;
  lastDisabled = scene;

  if (nDisabled == maxDisabled)
  {
    ESGE_Scene *nextDisabled = disabledList->next;

    delete disabledList;
    disabledList = nextDisabled;
  }
  else nDisabled++;
#ifndef ESGE_EDITOR
  scene->Disable();
#endif
}


ESGE_SceneMngr::ESGE_SceneMngr(void)
{}

ESGE_SceneMngr::~ESGE_SceneMngr(void)
{}


int ESGE_SceneMngr::maxDisabled;


void
ESGE_SceneMngr::Init(int maxDisabled)
{
  ESGE_SceneMngr::maxDisabled = maxDisabled;
}

void
ESGE_SceneMngr::Quit(void)
{
#ifndef ESGE_EDITOR
  for (ESGE_Scene *s = enabledList; s; s = s->next)
    s->Disable();
#endif

  for (ESGE_Scene *s = enabledList; s; s = s->next)
    delete s;

  for (ESGE_Scene *s = disabledList; s; s = s->next)
    delete s;
}


void
ESGE_SceneMngr::AddScene(const char *sceneFile)
{
  ESGE_Scene **node;
  Uint64 sceneID;

  sceneID = ESGE_Hash(sceneFile);

  for (
    node = &disabledList;
    *node && (*node)->id != sceneID;
    node = &(*node)->next
  );

  if (*node)
  {
    active = *node;

    *node = active->next;
    nDisabled--;
  }
  else active = new ESGE_Scene(sceneFile);

  EnableScene(active);
}

void
ESGE_SceneMngr::ChangeScene(const char *sceneFile)
{
  ESGE_Scene **node, *enabled;
  Uint64 sceneID;

  sceneID = ESGE_Hash(sceneFile);

  for (
    node = &disabledList;
    *node && (*node)->id != sceneID;
    node = &(*node)->next
  );

  if (*node)
  {
    active = *node;
    
    *node = active->next;
    nDisabled--;
  }
  else active = new ESGE_Scene(sceneFile);

  enabled = enabledList;

  while (enabled)
  {
    ESGE_Scene *next = enabled->next;

    DisableScene(enabled);

    enabled = next;
  }

  enabledList = NULL;
  EnableScene(active);
}


void
ESGE_SceneMngr::CloseScene(const char *sceneFile)
{
  ESGE_Scene **node;
  Uint64 sceneID;

  sceneID = ESGE_Hash(sceneFile);

  for (
    node = &enabledList;
    *node && (*node)->id != sceneID;
    node = &(*node)->next
  );

  if (*node && (*node)->id == sceneID)
  {
    ESGE_Scene *scene = *node;

    *node = (*node)->next;

    if (scene == active) active = *node;

    DisableScene(scene);
  }
}


ESGE_Scene*
ESGE_SceneMngr::GetActiveScene(void)
{
  return active;
}
