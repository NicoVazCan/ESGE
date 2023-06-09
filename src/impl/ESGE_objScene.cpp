#include "ESGE_objScene.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

ESGE_ObjInScene *ESGE_ObjInScene::list = NULL;

ESGE_ObjInScene::ESGE_ObjInScene(Uint16 id): ESGE_ObjSerial(id) {}

ESGE_ObjInScene::~ESGE_ObjInScene(void) {}

void
ESGE_ObjInScene::OnEnable(void)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjInScene,
    list,
    this,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

void
ESGE_ObjInScene::OnDisable(void)
{
  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjInScene,
    list,
    this,
    next
  );
}


ESGE_ObjSerial*
ESGE_LoadObjScene(SDL_RWops *io)
{
  Uint16 id, typeID;
  ESGE_ObjInScene *obj, *disList = NULL;

  id = SDL_ReadBE16(io);

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
      disList,
      obj,
      ESGE_CMP_OBJ_SERIAL,
      next
    );
  }

  return new ESGE_ObjScene(id, disList);
}

static ESGE_Loader ESGE_ObjSceneLoader(
  ESGE_OBJ_SCENE_TYPE_ID,
  ESGE_LoadObjScene
);

ESGE_ObjScene::ESGE_ObjScene(Uint16 id, ESGE_ObjInScene *disList):
  ESGE_ObjSerial(id),
  disList(disList)
{}

ESGE_ObjScene::~ESGE_ObjScene(void)
{
  SGLIB_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjInScene,
    disList,
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
  ESGE_ObjInScene *list = disList == NULL?
    ESGE_ObjInScene::list: disList;

  ESGE_ObjSerial::OnSave(io);

  SGLIB_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjInScene,
    list,
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
    disList,
    obj,
    next,
    {
      obj->OnEnable();
    }
  );

  disList = NULL;

  ESGE_ObjSerial::OnEnable();
}

void
ESGE_ObjScene::OnDisable(void)
{
  SGLIB_LIST_MAP_ON_ELEMENTS(
    ESGE_ObjInScene,
    ESGE_ObjInScene::list,
    obj,
    next,
    {
      obj->OnDisable();
      
      SGLIB_SORTED_LIST_ADD(
        ESGE_ObjInScene,
        disList,
        obj,
        ESGE_CMP_OBJ_SERIAL,
        next
      );
    }
  );

  ESGE_ObjSerial::OnDisable();
}
