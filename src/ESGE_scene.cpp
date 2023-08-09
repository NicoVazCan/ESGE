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


void
ESGE_ObjScene::OnEditorInit(void)
{}

void
ESGE_ObjScene::OnEditorQuit(void)
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
      ESGE_ObjScene *obj, *member;

      typeID = ESGE_ReadU64(io);
      if (!(obj = (ESGE_ObjScene*)ESGE_Type::New(typeID)))
      {
        ESGE_Error(
          "Cannot create object with typeID=%" SDL_PRIu64 " from "
          "file \"%s\": %s",
          typeID,
          fileName,
          SDL_GetError()
        );
      }
      ESGE_ReadStr(io, obj->instName, ESGE_INST_NAME_LEN);
      obj->Load(io);

      obj->sceneID = fileID;
      obj->instID = ESGE_Hash(obj->instName);

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
  }
  else SDL_ClearError();
}

ESGE_Scene::~ESGE_Scene(void)
{
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
ESGE_Scene::EditorInit(void)
{
  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
    obj->OnEditorInit();
}

void
ESGE_Scene::EditorQuit(void)
{
  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
    obj->OnEditorQuit();
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
  ESGE_ObjScene *obj, *member;
  Uint64 instCnt = 0;

  SDL_assert(typeName != NULL);

  if (!(obj = (ESGE_ObjScene*)ESGE_Type::New(typeName)))
  {
    SDL_SetError(
      "Cannot add object of type \"%s\" to \"%s\" scene: %s",
      typeName,
      fileName,
      SDL_GetError()
    );
    return NULL;
  }

  SDL_strlcpy(obj->instName, typeName, ESGE_INST_NAME_LEN);
  obj->instID = ESGE_Hash(obj->instName);
  obj->sceneID = fileID;

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

    (*node)->~ESGE_ObjScene();
    SDL_free(*node);

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

  if (obj != NULL && obj->instID == instID) return obj;
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
