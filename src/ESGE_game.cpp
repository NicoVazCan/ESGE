#include <SDL2/SDL.h>
#include "ESGE_file.h"
#include "ESGE_scene.h"
#include "ESGE_display.h"

#include "ESGE_objEvent.h"
#include "ESGE_objUpdate.h"
#include "ESGE_objPhysic.h"
#include "ESGE_objDraw.h"


Uint32 ESGE_deltaTm = 16;

#ifdef __cplusplus
extern "C"
#endif
int
main(int argc, const char *argv[])
{
	const char *title = "game", *sceneFile = "scene.bin";
	int w = 256, h = 144;
	ESGE_Scene *scene;
	Uint32 ticks, delta;

	for (int i = 1; i < argc; ++i)
	{
		if ('-' == argv[i][0])
		{
			if (i == argc-1)
			{
				puts("help");
				return 0;
			}
			switch (argv[i][1])
			{
			case 't':
				title = argv[++i];
				break;
			case 'w':
				SDL_sscanf(argv[++i], "%d", &w);
				break;
			case 'h':
				SDL_sscanf(argv[++i], "%d", &h);
				break;
			case 'f':
				SDL_sscanf(argv[++i], "%" SDL_PRIu32, &ESGE_deltaTm);
				ESGE_deltaTm = 1000 / ESGE_deltaTm;
				break;
			default:
				puts("help");
				return 0;
			}
		}
		else sceneFile = argv[i];
	}

	SDL_Init(SDL_INIT_VIDEO);
	ESGE_Display::Init(title, w, h);
	if (!(scene = ESGE_FileMngr<ESGE_Scene>::Watch(sceneFile)))
	{
		puts(SDL_GetError());
		SDL_ClearError();
		return -1;
	}
	scene->Enable();
	ticks = SDL_GetTicks();

	while (!ESGE_Event::quit)
	{
		ESGE_Event::Loop();

		ESGE_ObjUpdate::Update();
		ESGE_ObjPhysic::Physic();
		ESGE_ObjDraw::Draw();

		ESGE_Display::Update();

		ticks += delta = SDL_GetTicks() - ticks;
  	if (delta < ESGE_deltaTm) SDL_Delay(ESGE_deltaTm - delta);
	}

	scene->Disable();
	ESGE_FileMngr<ESGE_Scene>::Leave(scene);
	ESGE_Display::Quit();
	SDL_Quit();

	return 0;
}