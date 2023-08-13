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


ESGE_Scene::ESGE_Scene(const char *fileName): ESGE_File(fileName)
{
  SDL_RWops *io;

  if ((io = SDL_RWFromFile(fileName, "rb")))
  {
    Sint64 size, tell;

    if ((size = SDL_RWsize(io)) == -1)
    {
      ESGE_Error(
        "Cannot get file \"%s\" size: %s",
        fileName,
        SDL_GetError()
      );
    }
    if ((tell = SDL_RWtell(io)) == -1)
      ESGE_Error("Cannot get file \"%s\" position", fileName);

    while (tell < size)
    {
      Uint64 typeID;
      const ESGE_Type *type;
      ESGE_ObjScene *obj, *member;

      typeID = ESGE_ReadU64(io);

      if (!(type = ESGE_Type::Get(typeID)))
      {
        ESGE_Error(
          "Cannot find type of typeID=%" SDL_PRIu64 " from "
          "file \"%s\": %s",
          typeID,
          fileName,
          SDL_GetError()
        );
      }
      obj = (ESGE_ObjScene*)type->New();

      obj->sceneID = fileID;
      obj->typeID  = typeID;
      ESGE_ReadStr(io, obj->instName, ESGE_INST_NAME_LEN);
      obj->instID  = ESGE_Hash(obj->instName);

      obj->Load(io);

      SGLIB_SORTED_LIST_ADD_IF_NOT_MEMBER(
        ESGE_ObjScene,
        objList,
        obj,
        ESGE_ObjScene::Cmp,
        next,
        member
      );
      SDL_assert(
        !member ||
        member->instID != obj->instID ||
        !"Two objects of the same typeID and instName"
      );

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
  objList = NULL;
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

  if (!(io = SDL_RWFromFile(fileName, "wb")))
  {
    return SDL_SetError(
      "Cannot open file while saving \"%s\" scene: %s",
      fileName,
      SDL_GetError()
    );
  }

  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
  {
    ESGE_WriteU64(io, obj->typeID);
    ESGE_WriteStr(io, obj->instName, ESGE_INST_NAME_LEN);
    obj->Save(io);
  }
  SDL_RWclose(io);

  return 0;
}


ESGE_ObjScene*
ESGE_Scene::AddObj(const char *typeName)
{
  const ESGE_Type *type;
  ESGE_ObjScene *obj, *member;
  Uint64 typeID, instCnt = 0;

  SDL_assert(typeName != NULL);

  typeID = ESGE_Hash(typeName);

  if (!(type = ESGE_Type::Get(typeID)))
  {
    SDL_SetError(
      "Cannot find type \"%s\" in \"%s\" scene",
      typeName,
      fileName
    );
    return NULL;
  }
  obj = (ESGE_ObjScene*)type->New();

  obj->sceneID = fileID;
  obj->typeID  = typeID;
  SDL_strlcpy(obj->instName, typeName, ESGE_INST_NAME_LEN);
  obj->instID = ESGE_Hash(obj->instName);

  SGLIB_SORTED_LIST_ADD_IF_NOT_MEMBER(
    ESGE_ObjScene,
    objList,
    obj,
    ESGE_ObjScene::Cmp,
    next,
    member
  );

  while (member != NULL && member->instID == obj->instID)
  {
    SDL_snprintf(
      obj->instName,
      ESGE_INST_NAME_LEN,
      "%s%" SDL_PRIu64,
      typeName,
      instCnt++
    );
    obj->instID = ESGE_Hash(obj->instName);

    SGLIB_SORTED_LIST_ADD_IF_NOT_MEMBER(
      ESGE_ObjScene,
      objList,
      obj,
      ESGE_ObjScene::Cmp,
      next,
      member
    );
  }
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
      fileName
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
        fileName
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
    fileName
  );
}
#endif
