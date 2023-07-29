#include "ESGE_game.h"
#define SGLIB_ASSERT SDL_assert
#include "ESGE_plc.h"

Uint32 ESGE_deltaTime = ESGE_GAME_DEFAULT_DELTA;

void
ESGE_MainLoop(void)
{
  Uint32 ticks, delta;

  ticks = SDL_GetTicks();

  while (ESGE_AnyPLC())
  {
    ESGE_UpdatePLC();
    ticks += delta = SDL_GetTicks() - ticks;
    if (delta < ESGE_deltaTime) SDL_Delay(ESGE_deltaTime - delta);
  }
}
