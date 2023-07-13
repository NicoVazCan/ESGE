#include "ESGE_scene.h"
#define SGLIB_ASSERT SDL_assert
#include "sglib.h"

ESGE_ObjScene::ESGE_ObjScene(void) {}
ESGE_ObjScene::~ESGE_ObjScene(void) {}


ESGE_Scene::ESGE_Scene(const char *fileName): ESGE_File(fileName) {}

ESGE_Scene::~ESGE_Scene(void)
{
  SGLIB_SORTED_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjScene,
    objList,
    obj,
    next,
    {
      obj->~ESGE_ObjScene();
      SDL_free(obj);
    }
  );
  objList = NULL;
}


int
ESGE_Scene::Load(void)
{
  SDL_RWops *io;

  if ((io = SDL_RWFromFile(fileName, "rb")))
  {
    Sint64 size, tell;

    if ((size = SDL_RWsize(io)) == -1) return -1;
    if ((tell = SDL_RWtell(io)) == -1) return -1;

    while (tell < size)
    {
      Uint64 typeID;
      ESGE_ObjScene *obj, *member;

      if (ESGE_Read(io, &typeID))                           return -1;
      if (!(obj = (ESGE_ObjScene*)ESGE_Type::New(typeID)))  return -1;
      if (ESGE_Read(io, obj->instName, ESGE_INST_NAME_LEN)) return -1;
      if (obj->OnLoad(io))                                  return -1;

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
        member == NULL ||
        member->instID != obj->instID ||
        "Two objects of the same typeID and instName" == NULL
      );

      tell = SDL_RWtell(io);
    }
    SDL_RWclose(io);
  }
  else SDL_ClearError();

  return 0;
}

int
ESGE_Scene::Save(void)
{
  SDL_RWops *io;

  if (!(io = SDL_RWFromFile(fileName, "wb"))) return -1;

  for (ESGE_ObjScene *obj = objList; obj != NULL; obj = obj->next)
  {
    if (ESGE_Write(io, obj->typeID)) return -1;
    if (ESGE_Write(io, obj->instName, ESGE_INST_NAME_LEN)) return -1;
    if (obj->OnSave(io)) return -1;
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

  if ((obj = (ESGE_ObjScene*)ESGE_Type::New(typeName)) == NULL)
    return NULL;

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
    SDL_SetError("Object with instName=%s not found", instName);
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
      SDL_SetError("Object with instName=%s already exist", instName);
      return -1;
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

  SDL_SetError("Object with instName=%s not found", instName);
  return -1;
}
