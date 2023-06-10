#include "ESGE_objRoom.h"
#include "../sglib.h"

ESGE_ObjSerial*
ESGE_LoadObjRoomMngr(SDL_RWops *io)
{
  Uint16 id, x, camID;
  ESGE_Room *room, *unloadRooms = NULL;

  if(!(id = SDL_ReadBE16(io))) return NULL;

  camID = SDL_ReadBE16(io);

  while(SDL_RWread(io, &x, sizeof(Uint16), 1))
  {
    room = new ESGE_Room;

    room->scene = NULL;

    room->area.x = (unsigned)SDL_SwapBE16(x);
    room->area.y = (unsigned)SDL_ReadBE16(io);
    room->area.w = (unsigned)SDL_ReadBE16(io);
    room->area.h = (unsigned)SDL_ReadBE16(io);

    SDL_RWread(io, room->file, sizeof(char), ESGE_OBJ_ROOM_FILE_LEN);

    SGLIB_LIST_ADD(ESGE_Room, unloadRooms, room, next);
  }

  return new ESGE_ObjRoomMngr(id, camID, unloadRooms);
}

ESGE_ObjRoomMngr::ESGE_ObjRoomMngr(
  Uint16 id,
  Uint16 camID,
  ESGE_Room *unloadRooms
):
  ESGE_ObjInScene(id), camID(camID), unloadRooms(unloadRooms)
{}

ESGE_ObjRoomMngr::~ESGE_ObjRoomMngr(void) {}

void
ESGE_ObjRoomMngr::OnSave(SDL_RWops *io) const
{
  ESGE_ObjInScene::OnSave(io);
}

Uint16
ESGE_ObjRoomMngr::GetTypeID(void) const
{
  return ESGE_OBJ_ROOM_TYPE_ID;
}

void
ESGE_ObjRoomMngr::OnEnable(void) {}

void
ESGE_ObjRoomMngr::OnDisable(void) {}
