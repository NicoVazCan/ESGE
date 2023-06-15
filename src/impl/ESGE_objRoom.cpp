#include "ESGE_objRoom.h"
#include "ESGE_plc.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

#define ESGE_OBJ_ROOM_MNGR_LOAD_RANGE_H   32
#define ESGE_OBJ_ROOM_MNGR_LOAD_RANGE_V   32
#define ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE_H 16
#define ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE_V 16

ESGE_ObjRoomMngr *ESGE_ObjRoomMngr::list = NULL;

#define READ_STR(io, str) {                                         \
  for (char *_c=(str); (*_c = (char)SDL_ReadU8(io)) != '\0'; ++_c); \
}

#define WRITE_STR(io, str) {                                     \
  for (char *_c=(str); SDL_WriteU8(io, *_c) && *_c!='\0'; ++_c); \
}

static ESGE_ObjSerial*
ESGE_LoadObjRoomMngr(SDL_RWops *io)
{
  Uint16 id, nRooms;
  ESGE_Room *rooms;
  ESGE_ObjRoomMngr *obj;

  if(!(id = SDL_ReadBE16(io))) return NULL;
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

  for (
    obj = ESGE_ObjRoomMngr::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjRoomMngr(id, nRooms, rooms);
  }
  else return NULL;
}

static ESGE_Loader ESGE_ObjRoomMngrLoader(
  ESGE_OBJ_ROOM_MNGR_TYPE_ID,
  ESGE_LoadObjRoomMngr
);

static ESGE_ObjSerial*
ESGE_TextLoadObjRoomMngr(void)
{
  char buff[8];
  Uint16 id, nRooms;
  ESGE_Room *rooms;
  ESGE_ObjRoomMngr *obj;

  while (
    !(fputs("id: ", stdout), fgets(buff, 8, stdin)) ||
    !(id = SDL_atoi(buff))
  );
  while(!(fputs("nRooms: ", stdout), fgets(buff, 8, stdin)));
  nRooms = (unsigned)SDL_atoi(buff);

  rooms = new ESGE_Room[nRooms];

  for (Uint16 i = 0; i < nRooms; ++i)
  {
    while(!(printf("room[%hu].area.x: ", i), fgets(buff, 7, stdin)));
    rooms[i].area.x = SDL_atoi(buff);
    while(!(printf("room[%hu].area.y: ", i), fgets(buff, 7, stdin)));
    rooms[i].area.y = SDL_atoi(buff);
    while(!(printf("room[%hu].area.w: ", i), fgets(buff, 7, stdin)));
    rooms[i].area.w = SDL_atoi(buff);
    while(!(printf("room[%hu].area.h: ", i), fgets(buff, 7, stdin)));
    rooms[i].area.h = SDL_atoi(buff);

    while(
      !(printf("room[%hu].file: ", i),
        fgets(rooms[i].file, ESGE_OBJ_SCENE_FILE_LEN-1, stdin)
      )
    );

    rooms[i].enable = false;
    rooms[i].scene  = NULL;
  }

  for (
    obj = ESGE_ObjRoomMngr::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjRoomMngr(id, nRooms, rooms);
  }
  else return NULL;
}

static ESGE_TextLoader ESGE_ObjRoomMngrTextLoader(
  ESGE_OBJ_ROOM_MNGR_TYPE_ID,
  ESGE_TextLoadObjRoomMngr
);

ESGE_ObjRoomMngr::ESGE_ObjRoomMngr(
  Uint16 id,
  Uint16 nRooms,
  ESGE_Room *rooms
):
  ESGE_ObjInScene(id), nRooms(nRooms), rooms(rooms)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjRoomMngr,
    list,
    this,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

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

  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjRoomMngr,
    list,
    this,
    next
  );
}

void
ESGE_ObjRoomMngr::OnSave(SDL_RWops *io) const
{
  ESGE_ObjInScene::OnSave(io);
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
  for (Uint16 i = 0; i < nRooms; ++i)
  {
    if (rooms[i].scene != NULL && rooms[i].enable)
    {
      rooms[i].scene->Disable();
    }
  }
}

void
ESGE_ObjRoomMngr::SetCenter(SDL_Point center)
{
  const SDL_Rect loadArea = {
    center.x - ESGE_OBJ_ROOM_MNGR_LOAD_RANGE_H,
    center.y - ESGE_OBJ_ROOM_MNGR_LOAD_RANGE_V,
    ESGE_OBJ_ROOM_MNGR_LOAD_RANGE_H*2,
    ESGE_OBJ_ROOM_MNGR_LOAD_RANGE_V*2
  };

  const SDL_Rect enableArea = {
    center.x - ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE_H,
    center.y - ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE_V,
    ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE_H*2,
    ESGE_OBJ_ROOM_MNGR_ENABLE_RANGE_V*2
  };

  for (Uint16 i = 0; i < nRooms; ++i)
  {
    if (SDL_HasIntersection(&rooms[i].area, &loadArea))
    {
      if (rooms[i].scene == NULL)
      {
        rooms[i].scene = new ESGE_ObjScene(
          rooms[i].file
        );
      }
      if (SDL_HasIntersection(&rooms[i].area, &enableArea))
      {
        if (!rooms[i].enable)
        {
          rooms[i].enable = true;
          rooms[i].scene->Enable();
        }
      }
      else
      {
        if (rooms[i].enable)
        {
          rooms[i].enable = false;
          rooms[i].scene->Disable();
        }
      }
    }
    else
    {
      if (rooms[i].scene != NULL)
      {
        rooms[i].scene->Save();
        if (rooms[i].enable)
        {
          rooms[i].enable = false;
          rooms[i].scene->Disable();
        }
        delete rooms[i].scene;
        rooms[i].scene = NULL;
      }
    }
  }
}
