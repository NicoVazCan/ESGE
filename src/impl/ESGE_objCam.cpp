#include "ESGE_objCam.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

ESGE_ObjCam *ESGE_ObjCam::list = NULL;

static ESGE_ObjSerial*
ESGE_LoadObjCam(SDL_RWops *io)
{
  Uint16 id;
  SDL_Point pos;
  ESGE_ObjCam *obj;

  if(!(id = SDL_ReadBE16(io))) return NULL;

  pos.x = (unsigned)SDL_ReadBE16(io);
  pos.y = (unsigned)SDL_ReadBE16(io);

  for (
    obj = ESGE_ObjCam::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjCam(id, pos);
  }
  else return NULL;
}

static ESGE_Loader ESGE_ObjCamLoader(
  ESGE_OBJ_CAM_TYPE_ID,
  ESGE_LoadObjCam
);

static ESGE_ObjSerial*
ESGE_TextLoadObjCam(void)
{
  char buff[8];
  Uint16 id;
  SDL_Point pos;
  ESGE_ObjCam *obj;

  while (
    !(fputs("id: ", stdout), fgets(buff, 7, stdin)) ||
    !(id = (unsigned)SDL_atoi(buff))
  );
  while(!(fputs("pos.x: ", stdout), fgets(buff, 7, stdin)));
  pos.x = SDL_atoi(buff);
  while(!(fputs("pos.y: ", stdout), fgets(buff, 7, stdin)));
  pos.y = SDL_atoi(buff);

  for (
    obj = ESGE_ObjCam::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjCam(id, pos);
  }
  else return NULL;
}

static ESGE_TextLoader ESGE_ObjCamTextLoader(
  ESGE_OBJ_CAM_TYPE_ID,
  ESGE_TextLoadObjCam
);

ESGE_ObjCam::ESGE_ObjCam(Uint16 id, SDL_Point pos):
  ESGE_ObjInScene(id),
  ESGE_ObjPoint(pos)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjCam,
    list,
    this,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

ESGE_ObjCam::~ESGE_ObjCam(void)
{
  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjCam,
    list,
    this,
    next
  );
}

void
ESGE_ObjCam::OnSave(SDL_RWops *io) const
{
  ESGE_ObjInScene::OnSave(io);

  if (
    !SDL_WriteBE16(io, (unsigned)pos.x) ||
    !SDL_WriteBE16(io, (unsigned)pos.y)
  )
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
    SDL_assert("Failed to save ObjCam" == NULL);
  }
}

Uint16
ESGE_ObjCam::GetTypeID(void) const
{
  return ESGE_OBJ_CAM_TYPE_ID;
}

void
ESGE_ObjCam::SetCenter(SDL_Point pos)
{
  this->pos.x = ESGE_OBJ_CAM_CENTER_H_OFFSET + pos.x;
  this->pos.y = ESGE_OBJ_CAM_CENTER_V_OFFSET + pos.y;
}

SDL_Point
ESGE_ObjCam::WorldToDisplayPoint(SDL_Point pos) const
{
  pos.x = ESGE_ObjCam::WorldToPixel(pos.x - this->pos.x);
  pos.y = ESGE_ObjCam::WorldToPixel(pos.y - this->pos.y);

  return pos;
}

SDL_Point
ESGE_ObjCam::DisplayToWorldPoint(SDL_Point pos) const
{
  pos.x = this->pos.x + ESGE_ObjCam::PixelToWorld(pos.x);
  pos.y = this->pos.y + ESGE_ObjCam::PixelToWorld(pos.y);

  return pos;
}