#include "ESGE_objPlayer.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

# define ESGE_OBJ_PLAYER_W 160
# define ESGE_OBJ_PLAYER_H 320
# define ESGE_OBJ_PLAYER_ACC 2
# define ESGE_OBJ_PLAYER_MAX_VEL 20

ESGE_ObjPlayer *ESGE_ObjPlayer::list = NULL;

static ESGE_ObjSerial*
ESGE_LoadObjPlayer(SDL_RWops *io)
{
  Uint16 id, displayID, camID;
  SDL_Point pos;
  ESGE_ObjPlayer *obj;

  if(!(id = SDL_ReadBE16(io))) return NULL;

  pos.x = (unsigned)SDL_ReadBE16(io);
  pos.y = (unsigned)SDL_ReadBE16(io);
  displayID = SDL_ReadBE16(io);
  camID = SDL_ReadBE16(io);

  for (
    obj = ESGE_ObjPlayer::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjPlayer(id, pos, displayID, camID);
  }
  else return NULL;
}

static ESGE_Loader ESGE_ObjPlayerLoader(
  ESGE_OBJ_PLAYER_TYPE_ID,
  ESGE_LoadObjPlayer
);

static ESGE_ObjSerial*
ESGE_TextLoadObjPlayer(void)
{
  char buff[8];
  Uint16 id, displayID, camID;
  SDL_Point pos;
  ESGE_ObjPlayer *obj;

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
    obj = ESGE_ObjPlayer::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjPlayer(id, pos, displayID, camID);
  }
  else return NULL;
}

static ESGE_TextLoader ESGE_ObjPlayerTextLoader(
  ESGE_OBJ_PLAYER_TYPE_ID,
  ESGE_TextLoadObjPlayer
);

ESGE_ObjPlayer::ESGE_ObjPlayer(
  Uint16 id,
  SDL_Point pos,
  Uint16 displayID,
  Uint16 camID
):
  ESGE_ObjPoint(pos),
  ESGE_ObjInScene(id),
  ESGE_ObjKeyEvent(),
  ESGE_ObjMove(pos, {0,0}, {0,0}),
  ESGE_ObjDraw(ESGE_OBJ_PLAYER_LAYER),
  displayID(displayID),
  camID(camID)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjPlayer,
    list,
    this,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

ESGE_ObjPlayer::~ESGE_ObjPlayer(void)
{
  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjPlayer,
    list,
    this,
    next
  );
}

void
ESGE_ObjPlayer::OnEnable(void)
{
  ESGE_ObjDisplay *display;
  SDL_Window *wind;

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
  wind = SDL_RenderGetWindow(display->rend);
  windowID = SDL_GetWindowID(wind);

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
  ESGE_ObjKeyEvent::OnEnable();
  ESGE_ObjMove::OnEnable();
  ESGE_ObjDraw::OnEnable();
}

void
ESGE_ObjPlayer::OnDisable(void)
{
  ESGE_ObjInScene::OnDisable();
  ESGE_ObjKeyEvent::OnDisable();
  ESGE_ObjMove::OnDisable();
  ESGE_ObjDraw::OnDisable();
}

void
ESGE_ObjPlayer::OnSave(SDL_RWops *io) const
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
ESGE_ObjPlayer::GetTypeID(void) const
{
  return ESGE_OBJ_PLAYER_TYPE_ID;
}

void
ESGE_ObjPlayer::OnKeyDown(SDL_Keycode key, SDL_UNUSED SDL_Keymod mod)
{
  if (key == SDLK_LEFT)   vel.x = -ESGE_OBJ_PLAYER_MAX_VEL;
  if (key == SDLK_RIGHT)  vel.x = ESGE_OBJ_PLAYER_MAX_VEL;
  if (key == SDLK_UP)     vel.y = -ESGE_OBJ_PLAYER_MAX_VEL;
  if (key == SDLK_DOWN)   vel.y = ESGE_OBJ_PLAYER_MAX_VEL;
}

void
ESGE_ObjPlayer::OnKeyUp(SDL_Keycode key, SDL_UNUSED SDL_Keymod mod)
{
  if (key == SDLK_LEFT)   vel.x = 0;
  if (key == SDLK_RIGHT)  vel.x = 0;
  if (key == SDLK_UP)     vel.y = 0;
  if (key == SDLK_DOWN)   vel.y = 0;
}

void
ESGE_ObjPlayer::OnMove(void)
{
  //SDL_Point center;

  ESGE_ObjMove::OnMove();
/*
  center.x = pos.x + ESGE_OBJ_PLAYER_W/2;
  center.y = pos.y + ESGE_OBJ_PLAYER_H/2;

  cam->SetCenter(center);*/
}

void
ESGE_ObjPlayer::OnDraw(SDL_Renderer *rend)
{
  SDL_Point inDisplayPos;
  SDL_Rect body;

  inDisplayPos = cam->WorldToDisplayPoint(pos);
  body.x = inDisplayPos.x;
  body.y = inDisplayPos.y;
  body.w = ESGE_ObjCam::WorldToPixel(ESGE_OBJ_PLAYER_W);
  body.h = ESGE_ObjCam::WorldToPixel(ESGE_OBJ_PLAYER_H);

  SDL_SetRenderDrawColor(rend, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderFillRect(rend, &body);
}
