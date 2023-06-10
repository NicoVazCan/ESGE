#include "ESGE_objCam.h"
#include "../sglib.h"

#define ESGE_OBJ_CAM_W 256
#define ESGE_OBJ_CAM_H 144

ESGE_ObjCam *ESGE_ObjCam::list = NULL;

ESGE_ObjSerial*
ESGE_LoadObjCam(SDL_RWops *io)
{
  Uint16 id;
  SDL_Rect view;

  if(!(id = SDL_ReadBE16(io))) return NULL;

  view.x = (unsigned)SDL_ReadBE16(io);
  view.y = (unsigned)SDL_ReadBE16(io);
  view.w = ESGE_OBJ_CAM_W;
  view.h = ESGE_OBJ_CAM_H;

  return dynamic_cast<ESGE_ObjSerial*>(
    new ESGE_ObjCam(id, view)
  );
}

ESGE_ObjCam::ESGE_ObjCam(Uint16 id, SDL_Rect view):
  ESGE_ObjInScene(id),
  view(view)
{}

ESGE_ObjCam::~ESGE_ObjCam(void)
{}

void
ESGE_ObjCam::OnSave(SDL_RWops *io) const
{
  ESGE_ObjInScene::OnSave(io);

  if (
    !SDL_WriteBE16(io, (unsigned)view.x) ||
    !SDL_WriteBE16(io, (unsigned)view.y)
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
    SDL_assert("Failed to write ObjCam view" == NULL);
  }
}

Uint16
ESGE_ObjCam::GetTypeID(void) const
{
  return ESGE_OBJ_CAM_TYPE_ID;
}

void
ESGE_ObjCam::OnEnable(void)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjCam,
    list,
    this,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

void
ESGE_ObjCam::OnDisable(void)
{
  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjCam,
    list,
    this,
    next
  );
}
