#include "ESGE_event.h"
#include <SDL2/SDL.h>
#include "ESGE_plc.h"

void
OnQuit(void *n, SDL_Event *pE)
{
	SDL_Log("%s\n", __func__);
	ESGE_DelEventWatch(SDL_QUIT, OnQuit, n);
}

void
OnKeyDown(void *n, SDL_Event *pE)
{
	SDL_Log("%s\n", __func__);

	if (pE->key.keysym.sym == SDLK_0+*(int*)n)
	{
		SDL_Log("del\n");
		ESGE_DelEventWatch(SDL_KEYDOWN, OnKeyDown, n);
	}
}

void
OnKeyDown0(void *n, SDL_Event *pE)
{
	SDL_Log("%s\n", __func__);

	if (pE->key.keysym.sym == SDLK_0+*(int*)n)
	{
		SDL_Log("del\n");
		ESGE_DelEventWatch(SDL_KEYDOWN, OnKeyDown0, n);
	}
}

int
main(int argc, char const *argv[])
{
	SDL_Window *pWind;
	int n0, n1 = 0, n2 = 1;

	SDL_Init(SDL_INIT_VIDEO);

	pWind = SDL_CreateWindow(
    "Test",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    160,
    160,
    0
  );

  ESGE_AddEventWatch(SDL_QUIT, OnQuit, &n0);
  ESGE_AddEventWatch(SDL_KEYDOWN, OnKeyDown, &n1);
  ESGE_AddEventWatch(SDL_KEYDOWN, OnKeyDown, &n2);
  ESGE_AddEventWatch(SDL_KEYDOWN, OnKeyDown0, &n1);

	while (ESGE_AnyPLC())
	{
		ESGE_UpdatePLC();
		SDL_Delay(33);
	}

	SDL_DestroyWindow(pWind);

	SDL_Quit();

	return 0;
}