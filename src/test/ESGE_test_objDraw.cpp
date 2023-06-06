#include "ESGE_objDraw.h"
#include "ESGE_objEvent.h"
#include "ESGE_plc.h"
#include <SDL2/SDL.h>


class TestPlayer: ESGE_ObjDraw, ESGE_ObjKeyEvent, ESGE_ObjQuitEvent
{
  SDL_Rect pos = {0,0,16,16};
public:
  TestPlayer(SDL_Renderer *rend):
    ESGE_ObjDraw(1, rend),
    ESGE_ObjKeyEvent(),
    ESGE_ObjQuitEvent()
  {
    SDL_Log("%s", SDL_FUNCTION);
  }
  ~TestPlayer(void) override {SDL_Log("%s", SDL_FUNCTION);};
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
  virtual void OnQuit(void) override
  {
    delete this;
  }
  virtual void OnDraw(SDL_Renderer *rend) override
  {
    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(rend, &pos);
  }
};

class TestFloor: ESGE_ObjDraw, ESGE_ObjQuitEvent
{
  SDL_Rect pos = {16,16,160-16*2,160-16*2};
public:
  TestFloor(SDL_Renderer *rend):
    ESGE_ObjDraw(0, rend),
    ESGE_ObjQuitEvent()
  {
    SDL_Log("%s", SDL_FUNCTION);
  }
  ~TestFloor(void) override {SDL_Log("%s", SDL_FUNCTION);};
protected:
  virtual void OnQuit(void) override
  {
    delete this;
  }
  virtual void OnDraw(SDL_Renderer *rend) override
  {
    SDL_SetRenderDrawColor(rend, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(rend, &pos);
  }
};

int main(SDL_UNUSED int argc, SDL_UNUSED char const *argv[])
{
  SDL_Window *wind;
  SDL_Renderer *rend;

  SDL_Init(SDL_INIT_VIDEO);

  wind = SDL_CreateWindow(
    "Test",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    160,
    160,
    0
  );

  rend = SDL_CreateRenderer(wind, -1,  SDL_RENDERER_ACCELERATED);

  new TestPlayer(rend);
  new TestFloor(rend);

  while (ESGE_AnyPLC())
  {
    ESGE_UpdatePLC();
    SDL_Delay(33);
  }

  SDL_DestroyRenderer(rend);

  SDL_DestroyWindow(wind);

  SDL_Quit();

  return 0;
}
