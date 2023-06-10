#include "ESGE_objScene.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"


ESGE_ObjInScene::ESGE_ObjInScene(Uint16 id): ESGE_ObjSerial(id) {}

ESGE_ObjInScene::~ESGE_ObjInScene(void) {}


ESGE_ObjSerial*
ESGE_LoadObjScene(SDL_RWops *io)
{
  Uint16 id, typeID;
  ESGE_ObjInScene *obj, *objList = NULL;

  if(!(id = SDL_ReadBE16(io))) return NULL;

  while ((typeID = SDL_ReadBE16(io)))
  {
    SDL_assert(
      (
        obj = dynamic_cast<ESGE_ObjInScene*>(
          ESGE_Loader::Load(typeID, io)
        )
      ) != NULL ||
      "Failed to load ObjInScene" == NULL
    );

    SGLIB_SORTED_LIST_ADD(
      ESGE_ObjInScene,
      objList,
      obj,
      ESGE_CMP_OBJ_SERIAL,
      next
    );
  }

  return (
    dynamic_cast<ESGE_ObjSerial*>(new ESGE_ObjScene(id, objList))
  );
}

static ESGE_Loader ESGE_ObjSceneLoader(
  ESGE_OBJ_SCENE_TYPE_ID,
  ESGE_LoadObjScene
);

ESGE_ObjScene::ESGE_ObjScene(Uint16 id, ESGE_ObjInScene *objList):
  ESGE_ObjInScene(id),
  objList(objList)
{}

ESGE_ObjScene::~ESGE_ObjScene(void)
{
  SGLIB_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjInScene,
    objList,
    obj,
    next,
    {
      delete obj;
    }
  );
}

void
ESGE_ObjScene::OnSave(SDL_RWops *io) const
{
  ESGE_ObjSerial::OnSave(io);

  SGLIB_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjInScene,
    objList,
    obj,
    next,
    {
      if (!SDL_WriteBE16(io, obj->GetTypeID()))
      {
        SDL_LogError(
          SDL_LOG_CATEGORY_SYSTEM,
          "Error %s:%s:%d: %s",
          SDL_FILE,
          SDL_FUNCTION,
          SDL_LINE,
          SDL_GetError()
        );
        SDL_ClearError();
        SDL_assert("Failed to write scene obj typeID" == NULL);
      }
      obj->OnSave(io);
    }
  );
}

Uint16
ESGE_ObjScene::GetTypeID(void) const
{
  return ESGE_OBJ_SCENE_TYPE_ID;
}

void
ESGE_ObjScene::OnEnable(void)
{
  SGLIB_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjInScene,
    objList,
    obj,
    next,
    {
      obj->OnEnable();
    }
  );
  ESGE_ObjSerial::OnEnable();
}

void
ESGE_ObjScene::OnDisable(void)
{
  SGLIB_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjInScene,
    objList,
    obj,
    next,
    {
      obj->OnDisable();
    }
  );
  ESGE_ObjSerial::OnDisable();
}

void
ESGE_ObjScene::AddObj(ESGE_ObjInScene *obj)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjInScene,
    objList,
    obj,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

void
ESGE_ObjScene::DelObj(ESGE_ObjInScene *obj)
{
  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjInScene,
    objList,
    obj,
    next
  );
}
