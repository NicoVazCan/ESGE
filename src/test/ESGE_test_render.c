#include "ESGE_render.h"
#include "ESGE_event.h"
#include <SDL2/SDL.h>
#include "ESGE_plc.h"

void
OnDraw0(SDL_UNUSED void *userdata, SDL_Renderer *rend)
{
	static SDL_Rect pos = {0,0,16,16};

	pos.x = (pos.x+1)%(160-16);
	pos.y = (pos.y+1)%(160-16);

	SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(rend, &pos);
}

void
OnDraw1(SDL_UNUSED void *userdata, SDL_Renderer *rend)
{
	static SDL_Rect pos = {160-16,0,16,16};

	pos.x = pos.x > 0? pos.x-1: 160-16;
	pos.y = (pos.y+1)%(160-16);

	SDL_SetRenderDrawColor(rend, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderFillRect(rend, &pos);
}

void
OnQuit(void *p, SDL_UNUSED SDL_Event *e)
{
	ESGE_DelEventWatch(SDL_QUIT, OnQuit, p);
	ESGE_DelDrawCallback(0, p, OnDraw0, p);
	ESGE_DelDrawCallback(1, p, OnDraw1, p);
}

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

	ESGE_AddEventWatch(SDL_QUIT, OnQuit, rend);
	ESGE_AddDrawCallback(0, rend, OnDraw0, rend);
	ESGE_AddDrawCallback(1, rend, OnDraw1, rend);

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