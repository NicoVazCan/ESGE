#include "ESGE_objTile.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

# define ESGE_OBJ_TILE_W 160
# define ESGE_OBJ_TILE_H 160

ESGE_ObjTile *ESGE_ObjTile::list = NULL;

static ESGE_ObjSerial*
ESGE_LoadObjTile(SDL_RWops *io)
{
  Uint16 id, displayID, camID;
  SDL_Point pos;
  ESGE_ObjTile *obj;

  if(!(id = SDL_ReadBE16(io))) return NULL;

  pos.x = (unsigned)SDL_ReadBE16(io);
  pos.y = (unsigned)SDL_ReadBE16(io);
  displayID = SDL_ReadBE16(io);
  camID = SDL_ReadBE16(io);

  for (
    obj = ESGE_ObjTile::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjTile(id, pos, displayID, camID);
  }
  else return NULL;
}

static ESGE_Loader ESGE_ObjTileLoader(
  ESGE_OBJ_TILE_TYPE_ID,
  ESGE_LoadObjTile
);

static ESGE_ObjSerial*
ESGE_TextLoadObjTile(void)
{
  char buff[8];
  Uint16 id, displayID, camID;
  SDL_Point pos;
  ESGE_ObjTile *obj;

  while (
    !(fputs("id: ", stdout), fgets(buff, 7, stdin)) ||
    !(id = (unsigned)SDL_atoi(buff))
  );
  while(!(fputs("pos.x: ", stdout), fgets(buff, 7, stdin)));
  pos.x = SDL_atoi(buff);
  while(!(fputs("pos.y: ", stdout), fgets(buff, 7, stdin)));
  pos.y = SDL_atoi(buff);
  while(!(fputs("displayID: ", stdout), fgets(buff, 7, stdin)));
  displayID = (unsigned)SDL_atoi(buff);
  while(!(fputs("camID: ", stdout), fgets(buff, 7, stdin)));
  camID = (unsigned)SDL_atoi(buff);

  for (
    obj = ESGE_ObjTile::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjTile(id, pos, displayID, camID);
  }
  else return NULL;
}

static ESGE_TextLoader ESGE_ObjTileTextLoader(
  ESGE_OBJ_TILE_TYPE_ID,
  ESGE_TextLoadObjTile
);

ESGE_ObjTile::ESGE_ObjTile(
  Uint16 id,
  SDL_Point pos,
  Uint16 displayID,
  Uint16 camID
):
  ESGE_ObjInScene(id),
  ESGE_ObjPoint(pos),
  ESGE_ObjDraw(ESGE_OBJ_TILE_LAYER),
  displayID(displayID),
  camID(camID)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjTile,
    list,
    this,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

ESGE_ObjTile::~ESGE_ObjTile(void)
{
  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjTile,
    list,
    this,
    next
  );
}

void
ESGE_ObjTile::OnEnable(void)
{
  ESGE_ObjDisplay *display;

  for (
    display = ESGE_ObjDisplay::list;
    display != NULL && display->id < displayID;
    display = display->next
  );
  SDL_assert(
    (display != NULL && display->id == displayID) ||
    "ObjDisplay not found" == NULL
  );
  SetRenderer(display->rend);

  for (
    cam = ESGE_ObjCam::list;
    cam != NULL && cam->id < camID;
    cam = cam->next
  );
  SDL_assert(
    (cam != NULL && cam->id == camID) ||
    "ObjCam not found" == NULL
  );

  ESGE_ObjInScene::OnEnable();
  ESGE_ObjDraw::OnEnable();
}

void
ESGE_ObjTile::OnDisable(void)
{
  ESGE_ObjInScene::OnDisable();
  ESGE_ObjDraw::OnDisable();
}

void
ESGE_ObjTile::OnSave(SDL_RWops *io) const
{
  ESGE_ObjInScene::OnSave(io);

  if (
    !SDL_WriteBE16(io, (unsigned)pos.x) ||
    !SDL_WriteBE16(io, (unsigned)pos.y) ||
    !SDL_WriteBE16(io, displayID) ||
    !SDL_WriteBE16(io, camID)
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
    SDL_assert("Failed to save ObjTile" == NULL);
  }
}

Uint16
ESGE_ObjTile::GetTypeID(void) const
{
  return ESGE_OBJ_TILE_TYPE_ID;
}

void
ESGE_ObjTile::OnDraw(SDL_Renderer *rend)
{
  SDL_Point inDisplayPos;
  SDL_Rect tile;

  inDisplayPos = cam->WorldToDisplayPoint(pos);
  tile.x = inDisplayPos.x;
  tile.y = inDisplayPos.y;
  tile.w = ESGE_ObjCam::WorldToPixel(ESGE_OBJ_TILE_W);
  tile.h = ESGE_ObjCam::WorldToPixel(ESGE_OBJ_TILE_H);

  SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderFillRect(rend, &tile);
}
