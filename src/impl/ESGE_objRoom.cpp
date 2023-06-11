#include "ESGE_objRoom.h"
#include "../sglib.h"
#include "ESGE_plc.h"

#define ESGE_OBJ_ROOM_MNGR_LOAD_RANGE 32
#define ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE 16

#define READ_STR(io, str) {                                         \
  for (char *_c=(str); (*_c = (char)SDL_ReadU8(io)) != '\0'; ++_c); \
}

#define WRITE_STR(io, str) {                                     \
  for (char *_c=(str); SDL_WriteU8(io, *_c) && *_c!='\0'; ++_c); \
}

ESGE_ObjSerial*
ESGE_LoadObjRoomMngr(SDL_RWops *io)
{
  Uint16 id, camID, nRooms;
  ESGE_Room *rooms;

  if(!(id = SDL_ReadBE16(io))) return NULL;
  camID   = SDL_ReadBE16(io);
  nRooms  = SDL_ReadBE16(io);

  rooms = new ESGE_Room[nRooms];

  for (Uint16 i = 0; i < nRooms; ++i)
  {
    rooms[i].area.x = (unsigned)SDL_ReadBE16(io);
    rooms[i].area.y = (unsigned)SDL_ReadBE16(io);
    rooms[i].area.w = (unsigned)SDL_ReadBE16(io);
    rooms[i].area.h = (unsigned)SDL_ReadBE16(io);

    READ_STR(io, rooms[i].file);

    rooms[i].enable = false;
    rooms[i].scene  = NULL;
  }

  return new ESGE_ObjRoomMngr(id, camID, nRooms, rooms);
}

static ESGE_Loader ESGE_ObjRoomMngrLoader(
  ESGE_OBJ_ROOM_MNGR_TYPE_ID,
  ESGE_LoadObjRoomMngr
);

void
ESGE_ObjRoomMngrUpdate(void *userdata)
{
  ESGE_ObjRoomMngr *pThis = (ESGE_ObjRoomMngr*)userdata;

  SDL_Rect loadArea = {
    pThis->cam->view.x - ESGE_OBJ_ROOM_MNGR_LOAD_RANGE,
    pThis->cam->view.y - ESGE_OBJ_ROOM_MNGR_LOAD_RANGE,
    pThis->cam->view.w + ESGE_OBJ_ROOM_MNGR_LOAD_RANGE*2,
    pThis->cam->view.h + ESGE_OBJ_ROOM_MNGR_LOAD_RANGE*2
  };

  SDL_Rect enableArea = {
    pThis->cam->view.x - ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE,
    pThis->cam->view.y - ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE,
    pThis->cam->view.w + ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE*2,
    pThis->cam->view.h + ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE*2
  };



  for (Uint16 i = 0; i < pThis->nRooms; ++i)
  {
    if (SDL_HasIntersection(&pThis->rooms[i].area, &loadArea))
    {
      if (pThis->rooms[i].scene == NULL)
      {
        pThis->rooms[i].scene = new ESGE_ObjScene(
          pThis->rooms[i].file
        );
      }
      if (SDL_HasIntersection(&pThis->rooms[i].area, &enableArea))
      {
        if (!pThis->rooms[i].enable)
        {
          pThis->rooms[i].enable = true;
          pThis->rooms[i].scene->Enable();
        }
      }
      else
      {
        if (pThis->rooms[i].enable)
        {
          pThis->rooms[i].enable = false;
          pThis->rooms[i].scene->Disable();
        }
      }
    }
    else
    {
      if (pThis->rooms[i].scene != NULL)
      {
        pThis->rooms[i].scene->Save();
        if (pThis->rooms[i].enable)
        {
          pThis->rooms[i].enable = false;
          pThis->rooms[i].scene->Disable();
        }
        delete pThis->rooms[i].scene;
        pThis->rooms[i].scene = NULL;
      }
    }
  }
}


ESGE_ObjRoomMngr::ESGE_ObjRoomMngr(
  Uint16 id,
  Uint16 camID,
  Uint16 nRooms,
  ESGE_Room *rooms
):
  ESGE_ObjInScene(id), camID(camID), nRooms(nRooms), rooms(rooms)
{}

ESGE_ObjRoomMngr::~ESGE_ObjRoomMngr(void)
{
  for (Uint16 i = 0; i < nRooms; ++i)
  {
    if (rooms[i].scene != NULL)
    {
      if (rooms[i].enable)
      {
        rooms[i].enable = false;
        rooms[i].scene->Disable();
      }
      delete rooms[i].scene;
      rooms[i].scene = NULL;
    }
  }
  delete[] rooms;
}

void
ESGE_ObjRoomMngr::OnSave(SDL_RWops *io) const
{
  ESGE_ObjInScene::OnSave(io);

  SDL_WriteBE16(io, camID);
  SDL_WriteBE16(io, nRooms);

  for (Uint16 i = 0; i < nRooms; ++i)
  {
    SDL_WriteBE16(io, (unsigned)rooms[i].area.x);
    SDL_WriteBE16(io, (unsigned)rooms[i].area.y);
    SDL_WriteBE16(io, (unsigned)rooms[i].area.w);
    SDL_WriteBE16(io, (unsigned)rooms[i].area.h);

    WRITE_STR(io, rooms[i].file);
  }
}

Uint16
ESGE_ObjRoomMngr::GetTypeID(void) const
{
  return ESGE_OBJ_ROOM_MNGR_TYPE_ID;
}

void
ESGE_ObjRoomMngr::OnEnable(void)
{
  for (
    cam = ESGE_ObjCam::list;
    cam != NULL && cam->id < camID;
    cam = cam->next
  );

  SDL_assert(
    (cam != NULL && cam->id == camID) ||
    "ObjCam not found" == NULL
  );

  SDL_assert(
    !ESGE_AddPLC(
      ESGE_OBJ_ROOM_MNGR_UPDATE_PRI,
      ESGE_ObjRoomMngrUpdate,
      this
    )
  );

  for (Uint16 i = 0; i < nRooms; ++i)
  {
    if (rooms[i].scene != NULL && rooms[i].enable)
    {
      rooms[i].scene->Enable();
    }
  }
}

void
ESGE_ObjRoomMngr::OnDisable(void)
{
  cam = NULL;

  ESGE_DelPLC(
    ESGE_OBJ_ROOM_MNGR_UPDATE_PRI,
    ESGE_ObjRoomMngrUpdate,
    this
  );

  for (Uint16 i = 0; i < nRooms; ++i)
  {
    if (rooms[i].scene != NULL && rooms[i].enable)
    {
      rooms[i].scene->Disable();
    }
  }
}
