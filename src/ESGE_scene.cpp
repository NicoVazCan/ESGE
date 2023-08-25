#include "ESGE_scene.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_hash.h"
#include "ESGE_error.h"
#include "ESGE_io.h"

ESGE_ObjScene*
ESGE_ObjScene::Create(const char *typeName)
{
  ESGE_Scene *active;
  ESGE_ObjScene *newObj;

  if (!(active = ESGE_SceneMngr::GetActiveScene()))
  {
    ESGE_Error(
      "Failed to create an object of type \"%s\": No scene loaded",
      typeName
    );
  }
  else if ((newObj = active->AddObj(typeName)))
  {
    return newObj;
  }
  else
  {
    ESGE_Error(
      "Failed to create an object of type \"%s\": %s",
      typeName,
      SDL_GetError()
    );
  }
}

ESGE_ObjScene::ESGE_ObjScene(void)
{}
ESGE_ObjScene::~ESGE_ObjScene(void)
{}

void
ESGE_ObjScene::Destroy(void)
{
  ESGE_Scene *scene;

  if (!(scene = ESGE_SceneMngr::GetActiveScene()))
  {
    ESGE_Error(
      "Failed to destroy \"%s\" object: No scene loaded",
      instName
    );
  }
  else if (scene->id == sceneID)
    scene->DelObj(instName);
  else
  {
    ESGE_Scene *prevScene = scene;

    ESGE_SceneMngr::SetActiveScene(sceneID);

    scene = ESGE_SceneMngr::GetActiveScene();
    scene->DelObj(instName);

    ESGE_SceneMngr::SetActiveScene(prevScene->id);
  }
}

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

void
ESGE_ObjScene::OnStart(void)
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

      obj->state = ESGE_ObjScene::OK;
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

  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
    obj->OnStart();
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

  obj->state = ESGE_ObjScene::ADD;

  return obj;
}

void
ESGE_Scene::DelObj(const char *instName)
{
  ESGE_ObjScene *obj;
  Uint64 instID;

  SDL_assert(instName != NULL);

  instID = ESGE_Hash(instName);

  for (
    obj = objList;
    obj && obj->instID < instID;
    obj = obj->next
  );

  if (obj && obj->instID == instID)
    obj->state = ESGE_ObjScene::DEL;
}

void
ESGE_Scene::Update(void)
{
  ESGE_ObjScene **node;

  node = &objList;
  while (*node)
  {
    switch ((*node)->state)
    {
    case ESGE_ObjScene::OK:
      node = &(*node)->next;
      break;

    case ESGE_ObjScene::ADD:
      (*node)->state = ESGE_ObjScene::OK;
#ifdef ESGE_EDITOR
      (*node)->OnEditorInit();
#else
      (*node)->OnInit();
      (*node)->OnStart();
      (*node)->OnEnable();
#endif
      node = &(*node)->next;
      break;

    case ESGE_ObjScene::DEL:
      ESGE_ObjScene *next;

      (*node)->state = ESGE_ObjScene::OK;
#ifdef ESGE_EDITOR
      (*node)->OnEditorQuit();
#else
      (*node)->OnDisable();
      (*node)->OnQuit();
#endif
      next = (*node)->next;
      delete *node;
      *node = next;
      break;

    default:
      SDL_assert(0);
    }
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
ESGE_Scene *ESGE_SceneMngr::lastDisabled = NULL;



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
  ESGE_Scene *s, *n;

#ifndef ESGE_EDITOR
  s = enabledList;
  while (s)
  {
    n = s->next;
    s->Disable();
    s = n;
  }
#endif
  s = enabledList;
  while (s)
  {
    n = s->next;
    delete s;
    s = n;
  }
  s = lastDisabled;
  while (s)
  {
    n = s->next;
    delete s;
    s = n;
  }

  enabledList = NULL;
  lastDisabled = NULL;
}

void
ESGE_SceneMngr::Update(void)
{
  ESGE_Scene **node;
  int nDisabled = 0;

  for (
    ESGE_Scene *scene = enabledList;
    scene;
    scene = scene->next
  )
    scene->Update();

  node = &enabledList;
  while (*node)
  {
    ESGE_Scene *scene;

    switch ((*node)->state)
    {
    case ESGE_Scene::OK:
      node = &(*node)->next;
      break;
    case ESGE_Scene::DIS:
      scene = *node;
      *node = scene->next;

      if (active == scene) active = *node;

      scene->next = lastDisabled;
      lastDisabled = scene;

#ifndef ESGE_EDITOR
      scene->Disable();
#endif

      scene->state = ESGE_Scene::OK;
      break;
    case ESGE_Scene::CLO:
      scene = *node;
      *node = scene->next;

      if (active == scene) active = *node;

#ifndef ESGE_EDITOR
      scene->Disable();
#endif
      
      delete scene;
      break;
    default:
      SDL_assert(0);
    }
  }

  node = &lastDisabled;
  while (*node)
  {
    ESGE_Scene *scene;

    switch ((*node)->state)
    {
    case ESGE_Scene::OK:
      node = &(*node)->next;
      break;
    case ESGE_Scene::EN:
      scene = *node;
      *node = scene->next;

      scene->next = enabledList;
      enabledList = scene;

      active = scene;

#ifndef ESGE_EDITOR
      scene->Enable();
#endif

      active->state = ESGE_Scene::OK;
      break;
    case ESGE_Scene::CLO:
      scene = *node;
      *node = scene->next;

      delete scene;
      break;
    default:
      SDL_assert(0);
    }
  }

  for (ESGE_Scene *scene = lastDisabled; scene; scene = scene->next)
  {
    if (nDisabled == maxDisabled)
    {
      ESGE_Scene *firstDisabled = scene, *next;

      scene = scene->next;

      while (scene)
      {
        next = scene->next;
        delete scene;
        scene = next;
      }

      firstDisabled->next = NULL;
    }
    else nDisabled++;
  }
}


void
ESGE_SceneMngr::AddScene(const char *sceneFile)
{
  ESGE_Scene *disabled;
  Uint64 sceneID;

  sceneID = ESGE_Hash(sceneFile);

  for (
    disabled = lastDisabled;
    disabled && disabled->id != sceneID;
    disabled = disabled->next
  );

  if (!disabled)
  {
    disabled = new ESGE_Scene(sceneFile);

    disabled->next = lastDisabled;
    lastDisabled = disabled;
  }

  disabled->state = ESGE_Scene::EN;
}


void
ESGE_SceneMngr::ChangeScene(const char *sceneFile)
{
  ESGE_SceneMngr::AddScene(sceneFile);

  for (
    ESGE_Scene *enabled = enabledList;
    enabled;
    enabled = enabled->next
  )
    enabled->state = ESGE_Scene::DIS;
}


void
ESGE_SceneMngr::StashScene(const char *sceneFile)
{
  StashScene(ESGE_Hash(sceneFile));
}

void
ESGE_SceneMngr::StashScene(Uint64 id)
{
  ESGE_Scene *enabled;

  for (
    enabled = enabledList;
    enabled && enabled->id != id;
    enabled = enabled->next
  );

  if (enabled && enabled->id == id)
    enabled->state = ESGE_Scene::DIS;
}


void
ESGE_SceneMngr::CloseScene(const char *sceneFile)
{
  CloseScene(ESGE_Hash(sceneFile));
}

void
ESGE_SceneMngr::CloseScene(Uint64 id)
{
  ESGE_Scene *scene;

  for (
    scene = enabledList;
    scene && scene->id != id;
    scene = scene->next
  );

  if (!scene)
  {
    for (
      scene = lastDisabled;
      scene && scene->id != id;
      scene = scene->next
    );
  }
  if (scene)
    scene->state = ESGE_Scene::CLO;
}


ESGE_Scene*
ESGE_SceneMngr::GetActiveScene(void)
{
  return active;
}

int
ESGE_SceneMngr::SetActiveScene(const char *sceneFile)
{
  return (
    SetActiveScene(ESGE_Hash(sceneFile)) ?
    SDL_SetError("Scene \"%s\" not loaded", sceneFile) :
    0
  );
}

int
ESGE_SceneMngr::SetActiveScene(Uint64 id)
{
  ESGE_Scene *scene;

  for (
    scene = enabledList;
    scene && scene->id != id;
    scene = scene->next
  );

  if (scene && scene->id == id)
  {
    active = scene;
    return 0;
  }
  return SDL_SetError(
    "Scene with id=\"%" SDL_PRIu64 "\" not loaded", id
  );
}
