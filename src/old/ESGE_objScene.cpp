#include "ESGE_objScene.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

#define ESGE_CMP_OBJ_SCENE(left, right) ( \
  SDL_strcmp((left)->file, (right)->file) \
)


ESGE_ObjInScene::ESGE_ObjInScene(Uint16 id): ESGE_ObjSerial(id) {}

ESGE_ObjInScene::~ESGE_ObjInScene(void) {}


ESGE_ObjScene *ESGE_ObjScene::list = NULL;

ESGE_ObjScene::ESGE_ObjScene(const char *file)
{
  SDL_strlcpy(this->file, file, ESGE_OBJ_SCENE_FILE_LEN);

  SDL_RWops *io;
  Uint16 typeID;
  ESGE_ObjSerial *obj;

  if ((io = SDL_RWFromFile(file, "rb")) != NULL)
  {
    while ((typeID = SDL_ReadBE16(io)))
    {
      if ((obj = ESGE_Loader::Load(typeID, io)) != NULL)
      {
        AddObj(dynamic_cast<ESGE_ObjInScene*>(obj));
      }
    }
  }

  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjScene,
    list,
    this,
    ESGE_CMP_OBJ_SCENE,
    next
  );
}

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

  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjScene,
    list,
    this,
    next
  );
}

void
ESGE_ObjScene::Save(void)
{
  SDL_RWops *io;

  SDL_assert(
    (io = SDL_RWFromFile(file, "wb")) != NULL ||
    "Failed to open or create file" == NULL
  );

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
        SDL_assert("Failed to write scene ObjInScene typeID" == NULL);
      }
      obj->OnSave(io);
    }
  );

  SDL_RWclose(io);
}

void
ESGE_ObjScene::Enable(void)
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
}

void
ESGE_ObjScene::Disable(void)
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