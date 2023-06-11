#include <SDL2/SDL.h>
#include "ESGE_objCam.h"
#include "ESGE_objRoom.h"
#include "ESGE_plc.h"
#include "ESGE_objDraw.h"
#include "ESGE_objEvent.h"
#include "ESGE_objSerial.h"
#include "ESGE_objDisplay.h"


#define ESGE_TEST_FLOOR_TYPE_ID 3


class TestFloor: ESGE_ObjInScene, ESGE_ObjDraw
{
  SDL_Rect pos;
  const Uint16 displayID;
  const Uint8 r,g,b;

  friend ESGE_ObjSerial *ESGE_LoadTestFloor(SDL_RWops *io);

public:
  TestFloor(
  	Uint16 id,
  	Uint16 displayID,
  	SDL_Rect pos,
  	Uint8 r,
  	Uint8 g,
  	Uint8 b
  ):
    ESGE_ObjInScene(id),
    ESGE_ObjDraw(0),
    pos(pos),
    displayID(displayID),
    r(r),g(g),b(b)
  {
    SDL_Log("%s id=%d", SDL_FUNCTION, id);
  }

  ~TestFloor(void) override
  {
    SDL_Log("%s id=%d", SDL_FUNCTION, id);
  }

  virtual void OnSave(SDL_RWops *io) const override
  {
  	SDL_Log("%s id=%d", SDL_FUNCTION, id);
    ESGE_ObjSerial::OnSave(io);

    if (!SDL_WriteBE16(io, displayID))
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
      SDL_assert("Failed to save TestPlayer displayID" == NULL);
    }

    SDL_WriteBE16(io, (unsigned)pos.x);
    SDL_WriteBE16(io, (unsigned)pos.y);
    SDL_WriteU8(io, r);
    SDL_WriteU8(io, g);
    SDL_WriteU8(io, b);
  }

  virtual Uint16 GetTypeID(void) const override
  {
    return ESGE_TEST_FLOOR_TYPE_ID;
  }

  virtual void OnEnable(void) override
  {
    ESGE_ObjDisplay *p;

    SDL_Log("%s id=%d", SDL_FUNCTION, id);

    for (
      p = ESGE_ObjDisplay::list;
      p != NULL && p->id < displayID;
      p = p->next
    );

    SDL_assert(
      (p != NULL && p->id == displayID) ||
      "Failed to find display" == NULL
    );

    SetRenderer(p->rend);

    ESGE_ObjInScene::OnEnable();
    ESGE_ObjDraw::OnEnable();
  }

  virtual void OnDisable(void) override
  {
  	SDL_Log("%s id=%d", SDL_FUNCTION, id);

    ESGE_ObjInScene::OnEnable();
    ESGE_ObjDraw::OnDisable();
  }

protected:
  virtual void OnDraw(SDL_Renderer *rend) override
  {
    SDL_SetRenderDrawColor(rend, r, g, b, 0xFF);
    SDL_RenderFillRect(rend, &pos);
  }
};

ESGE_ObjSerial *ESGE_LoadTestFloor(SDL_RWops *io)
{
  Uint16 id;
  Uint16 displayID;
  SDL_Rect pos;
  Uint8 r,g,b;

  if(!(id   = SDL_ReadBE16(io))) return NULL;
  displayID = SDL_ReadBE16(io);
  pos.x = (unsigned)SDL_ReadBE16(io);
  pos.y = (unsigned)SDL_ReadBE16(io);
  pos.w = 32;
  pos.h = 32;

  r = SDL_ReadU8(io);
  g = SDL_ReadU8(io);
  b = SDL_ReadU8(io);

  return new TestFloor(id, displayID, pos, r,g,b);
}

static ESGE_Loader ESGE_TestFloorLoader(
  ESGE_TEST_FLOOR_TYPE_ID,
  ESGE_LoadTestFloor
);


void TestGameUpdate(void *userdata);

class TestGame: ESGE_ObjQuitEvent, ESGE_ObjKeyEvent, ESGE_ObjDraw
{
	friend void TestGameUpdate(void *userdata);
	ESGE_ObjDisplay *display;
  ESGE_ObjRoomMngr *roomMngr;
  ESGE_ObjCam *cam;
  bool paused = false;
public:
  TestGame(void): ESGE_ObjDraw(10)
  {
  	const Uint8 data[] = {
  		0x00,0x01, //ObjDisplay::id
  		0x00,						//ObjDisplay::full
  		0x00,						//ObjDisplay::vsync

  		0x00,0x01, //ObjCam::id
  		0x00,0x00, //ObjCam::x
  		0x00,0x00, //ObjCam::y

  		0x00,0x02, //ObjRoomMngr::id
  		0x00,0x01, //ObjRoomMngr::camID
  		0x00,0x03, //ObjRoomMngr::nRoom

  		0x00,0x00, //ObjRoomMngr::room[0].area.x
  		0x00,0x00, //ObjRoomMngr::room[0].area.y
  		0x00,0x20, //ObjRoomMngr::room[0].area.w
  		0x00,0x20, //ObjRoomMngr::room[0].area.h
  		'r','o','o','m','0','.','b','i','n','\0',

  		0x00,0x20, //ObjRoomMngr::room[1].area.x
  		0x00,0x00, //ObjRoomMngr::room[1].area.y
  		0x00,0x20, //ObjRoomMngr::room[1].area.w
  		0x00,0x20, //ObjRoomMngr::room[1].area.h
  		'r','o','o','m','1','.','b','i','n','\0',

  		0x00,0x40, //ObjRoomMngr::room[2].area.x
  		0x00,0x00, //ObjRoomMngr::room[2].area.y
  		0x00,0x20, //ObjRoomMngr::room[2].area.w
  		0x00,0x20, //ObjRoomMngr::room[2].area.h
  		'r','o','o','m','2','.','b','i','n','\0',
  	};
  	SDL_RWops *io;


    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_assert((io = SDL_RWFromConstMem(data, sizeof(data))));

    display 	= dynamic_cast<ESGE_ObjDisplay*>(
    	ESGE_Loader::Load(ESGE_OBJ_DISPLAY_TYPE_ID, io)
    );
    cam 			= dynamic_cast<ESGE_ObjCam*>(
    	ESGE_Loader::Load(ESGE_OBJ_CAM_TYPE_ID, io)
    );
    roomMngr 	= dynamic_cast<ESGE_ObjRoomMngr*>(
    	ESGE_Loader::Load(ESGE_OBJ_ROOM_MNGR_TYPE_ID, io)
    );

    SetRenderer(display->rend);

    SDL_RWclose(io);

    OnEnable();
  }

  virtual ~TestGame(void) override
  {
    OnDisable();

    delete roomMngr;

    SDL_Quit();
  }

  virtual void OnEnable(void) override
  {
  	display->OnEnable();
  	cam->OnEnable();
    roomMngr->OnEnable();
    ESGE_ObjQuitEvent::OnEnable();
    ESGE_ObjKeyEvent::OnEnable();
    ESGE_ObjDraw::OnEnable();

    ESGE_AddPLC(
    	ESGE_OBJ_ROOM_MNGR_UPDATE_PRI+1,
    	TestGameUpdate,
    	this
    );
  }

  virtual void OnDisable(void) override
  {
  	display->OnDisable();
  	cam->OnDisable();
    roomMngr->OnDisable();
    ESGE_ObjQuitEvent::OnDisable();
    ESGE_ObjKeyEvent::OnDisable();
    ESGE_ObjDraw::OnDisable();

    ESGE_DelPLC(
    	ESGE_OBJ_ROOM_MNGR_UPDATE_PRI+1,
    	TestGameUpdate,
    	this
    );
  }

  void MainLoop(void)
  {
  	Uint32 ticks, delta;

  	ticks = SDL_GetTicks();

    while (ESGE_AnyPLC())
    {
      ESGE_UpdatePLC();
      ticks += delta = SDL_GetTicks() - ticks;
   	 	if (delta < 33) SDL_Delay(33 - delta);
    }
  }

protected:
  virtual void OnQuit(void) override
  {
    delete this;
  }

  virtual void OnKeyDown(
    SDL_Keycode key,
    SDL_UNUSED SDL_Keymod mod
  ) override
  {
    switch (key)
    {
    case SDLK_ESCAPE:
    	if ((paused = !paused))
      {
        roomMngr->OnDisable();
      }
      else
      {
        roomMngr->OnEnable();
      }
    	break;
    default:
      break;
    }
  }

  virtual void OnDraw(SDL_Renderer *rend) override
  {
  	SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
  	SDL_RenderDrawRect(rend, &cam->view);
  }
};

void TestGameUpdate(void *userdata)
{
	TestGame *pThis = (TestGame*)userdata;
	const Uint8 *keys;

	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_LEFT]) pThis->cam->view.x--;
	if (keys[SDL_SCANCODE_RIGHT]) pThis->cam->view.x++;
	if (keys[SDL_SCANCODE_UP]) pThis->cam->view.y--;
	if (keys[SDL_SCANCODE_DOWN]) pThis->cam->view.y++;
}

int main(SDL_UNUSED int argc, SDL_UNUSED char const *argv[])
{
  TestGame *game = new TestGame;

  game->MainLoop();

  return 0;
}