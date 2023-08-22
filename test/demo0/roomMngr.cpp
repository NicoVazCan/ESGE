#include "roomMngr.h"

#include "ESGE_display.h"

#define FOCUS_W (256+16*2)
#define FOCUS_H (144+16*2)


static Room _rooms[] = {
  {{0,       0, 256*2, 144}, "room0.bin", false},
  {{256*2*1, 0, 256*2, 144}, "room1.bin", false},
  {{256*2*2, 0, 256*2, 144}, "room2.bin", false}
};


void
ObjRoomMngr::LoadRooms(void)
{
  for (size_t i = 0; i < nRooms; ++i)
  {
    if (SDL_HasIntersection(&rooms[i].area, &focus))
    {
      if (!rooms[i].loaded)
      {
        ESGE_SceneMngr::AddScene(rooms[i].sceneFile);
        rooms[i].loaded = true;
      }
    }
    else
    {
      if (rooms[i].loaded)
      {
        ESGE_SceneMngr::StashScene(rooms[i].sceneFile);
        rooms[i].loaded = false;
      }
    }
  }
}


ObjRoomMngr::ObjRoomMngr(void):
  focus{0, 0, FOCUS_W, FOCUS_H},
  rooms(_rooms),
  nRooms(SDL_arraysize(_rooms))
{}

ObjRoomMngr::~ObjRoomMngr(void)
{}


void
ObjRoomMngr::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  ESGE_ShareObj<ObjRoomMngr>(this);

  ObjRoomMngr::LoadRooms();
}

void
ObjRoomMngr::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  ESGE_UnshareObj<ObjRoomMngr>(this);

  for (size_t i = 0; i < nRooms; ++i)
    rooms[i].loaded = false;
}


void
ObjRoomMngr::SetFocusCenter(int x, int y)
{
  x -= FOCUS_W / 2;
  y -= FOCUS_H / 2;

  if (focus.x != x || focus.y != y)
  {
    focus.x = x;
    focus.y = y;

    ObjRoomMngr::LoadRooms();
  }
  /*
  ESGE_Display::WorldDrawRect(focus, 0, 255, 0, 255);
  */
}


ESGE_TYPE(ObjRoomMngr)
