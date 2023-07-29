#include <SDL2/SDL.h>
#include "ESGE_plc.h"
#include "ESGE_objDraw.h"
#include "ESGE_objEvent.h"
#include "ESGE_objSerial.h"
#include "ESGE_objDisplay.h"
#include "ESGE_objScene.h"


#define ESGE_TEST_PLAYER_TYPE_ID 3

class TestPlayer:
  ESGE_ObjInScene,
  ESGE_ObjDraw,
  ESGE_ObjKeyEvent
{

  SDL_Rect pos;
  const Uint16 displayID;

  friend ESGE_ObjSerial *ESGE_LoadTestPlayer(SDL_RWops *io);

public:
  TestPlayer(
    Uint16 id,
    SDL_Rect pos,
    Uint16 displayID
  ):
    ESGE_ObjInScene(id),
    ESGE_ObjDraw(1),
    ESGE_ObjKeyEvent(),
    pos(pos),
    displayID(displayID)
  {
    SDL_Log("%s", SDL_FUNCTION);
  }

  ~TestPlayer(void) override
  {
    SDL_Log("%s", SDL_FUNCTION);
  }

  virtual void OnSave(SDL_RWops *io) const override
  {
    ESGE_ObjSerial::OnSave(io);

    if (!SDL_WriteBE16(io, (unsigned)pos.x))
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
      SDL_assert("Failed to save TestPlayer pos.x" == NULL);
    }
    if (!SDL_WriteBE16(io, (unsigned)pos.y))
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
      SDL_assert("Failed to save TestPlayer pos.y" == NULL);
    }

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
  }

  virtual Uint16 GetTypeID(void) const override
  {
    return ESGE_TEST_PLAYER_TYPE_ID;
  }

  virtual void OnEnable(void) override
  {
    ESGE_ObjDisplay *p;

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
    ESGE_ObjKeyEvent::OnEnable();
  }

  virtual void OnDisable(void) override
  {
    ESGE_ObjInScene::OnDisable();
    ESGE_ObjDraw::OnDisable();
    ESGE_ObjKeyEvent::OnDisable();
  }

protected:
  virtual void OnKeyDown(
    SDL_Keycode key,
    SDL_UNUSED SDL_Keymod mod
  ) override
  {
    switch (key)
    {
    case SDLK_LEFT:
      pos.x--;
      break;
    case SDLK_RIGHT:
      pos.x++;
      break;
    case SDLK_UP:
      pos.y--;
      break;
    case SDLK_DOWN:
      pos.y++;
      break;
    default:
      break;
    }
  }

  virtual void OnKeyUp(
    SDL_UNUSED SDL_Keycode key,
    SDL_UNUSED SDL_Keymod mod
  ) override {}

  virtual void OnDraw(SDL_Renderer *rend) override
  {
    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(rend, &pos);
  }
};

ESGE_ObjSerial *ESGE_LoadTestPlayer(SDL_RWops *io)
{
  Uint16 id;
  SDL_Rect pos = {0,0,16,16};
  Uint16 displayID;

  if(!(id   = SDL_ReadBE16(io))) return NULL;
  pos.x     = SDL_ReadBE16(io);
  pos.y     = SDL_ReadBE16(io);
  displayID = SDL_ReadBE16(io);

  return new TestPlayer(id, pos, displayID);
}

static ESGE_Loader ESGE_TestPlayerLoader(
  ESGE_TEST_PLAYER_TYPE_ID,
  ESGE_LoadTestPlayer
);


#define ESGE_TEST_FLOOR_TYPE_ID 4

class TestFloor: ESGE_ObjInScene, ESGE_ObjDraw
{
  SDL_Rect pos = {16,16,160-16*2,160-16*2};
  const Uint16 displayID;

  friend ESGE_ObjSerial *ESGE_LoadTestFloor(SDL_RWops *io);

public:
  TestFloor(Uint16 id, Uint16 displayID):
    ESGE_ObjInScene(id),
    ESGE_ObjDraw(0),
    displayID(displayID)
  {
    SDL_Log("%s", SDL_FUNCTION);
  }

  ~TestFloor(void) override
  {
    SDL_Log("%s", SDL_FUNCTION);
  }

  virtual void OnSave(SDL_RWops *io) const override
  {
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
  }

  virtual Uint16 GetTypeID(void) const override
  {
    return ESGE_TEST_FLOOR_TYPE_ID;
  }

  virtual void OnEnable(void) override
  {
    ESGE_ObjDisplay *p;

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
    ESGE_ObjInScene::OnEnable();
    ESGE_ObjDraw::OnDisable();
  }

protected:
  virtual void OnDraw(SDL_Renderer *rend) override
  {
    SDL_SetRenderDrawColor(rend, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(rend, &pos);
  }
};

ESGE_ObjSerial *ESGE_LoadTestFloor(SDL_RWops *io)
{
  Uint16 id;
  Uint16 displayID;

  if(!(id   = SDL_ReadBE16(io))) return NULL;
  displayID = SDL_ReadBE16(io);

  return new TestFloor(id, displayID);
}

static ESGE_Loader ESGE_TestFloorLoader(
  ESGE_TEST_FLOOR_TYPE_ID,
  ESGE_LoadTestFloor
);



class TestGame: ESGE_ObjQuitEvent, ESGE_ObjKeyEvent
{
  ESGE_ObjScene *scene;
  bool paused = false;
public:
  TestGame(void)
  {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    scene = new ESGE_ObjScene("test_scene.bin");

    OnEnable();
  }

  virtual ~TestGame(void) override
  {
    OnDisable();

    delete scene;

    SDL_Quit();
  }

  virtual void OnEnable(void) override
  {
    scene->Enable();
    ESGE_ObjQuitEvent::OnEnable();
    ESGE_ObjKeyEvent::OnEnable();
  }

  virtual void OnDisable(void) override
  {
    scene->Disable();
    ESGE_ObjQuitEvent::OnDisable();
    ESGE_ObjKeyEvent::OnDisable();
  }

  void MainLoop(void)
  {
    while (ESGE_AnyPLC())
    {
      ESGE_UpdatePLC();
      SDL_Delay(33);
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
    if (key == SDLK_ESCAPE)
    {
      if ((paused = !paused))
      {
        scene->Disable();
      }
      else
      {
        scene->Enable();
      }
    }
    else if (key == SDLK_g)
    {
      scene->Save();
    }
  }
};


int main(SDL_UNUSED int argc, SDL_UNUSED char const *argv[])
{
  TestGame *game = new TestGame;

  game->MainLoop();

  return 0;
}