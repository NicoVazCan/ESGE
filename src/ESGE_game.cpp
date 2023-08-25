#include <SDL2/SDL.h>
#include "ESGE_file.h"
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_audio.h"

#include "ESGE_objEvent.h"
#include "ESGE_objUpdate.h"
#include "ESGE_objPhysic.h"
#include "ESGE_objDraw.h"


Uint32 ESGE_deltaTm = 16, ESGE_realDeltaTm;

#ifdef __cplusplus
extern "C"
#endif
int
main(int argc, const char *argv[])
{
	const char *title = "game", *sceneFile = "scene.bin";
	int w = 256, h = 144, nDisabledScene = 4;
	Uint8 soundVolume = 0x0F, musicVolume = 0x0F;
	Uint32 ticks;

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
			case 'd':
				SDL_sscanf(argv[++i], "%d", &nDisabledScene);
				break;
			case 's':
				SDL_sscanf(argv[++i], "%hhu", &soundVolume);
				break;
			case 'm':
				SDL_sscanf(argv[++i], "%hhu", &musicVolume);
				break;
			default:
				puts("help");
				return 0;
			}
		}
		else sceneFile = argv[i];
	}

	SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	ESGE_Display::Init(title, w, h);
	ESGE_InitAudio(soundVolume, musicVolume);
	ESGE_SceneMngr::Init(nDisabledScene);
	ESGE_SceneMngr::AddScene(sceneFile);

	ticks = SDL_GetTicks();

	for (;;)
	{
		ESGE_EventLoop();
		if (ESGE_quit) break;

		ESGE_ObjUpdate::Update();
		ESGE_ObjPhysic::Physic();
		ESGE_SceneMngr::Update();
		ESGE_ObjDraw::Draw();

		ESGE_Display::Update();

		ticks += ESGE_realDeltaTm = SDL_GetTicks() - ticks;
  	if (ESGE_realDeltaTm < ESGE_deltaTm)
  		SDL_Delay(ESGE_deltaTm - ESGE_realDeltaTm);
	}

	ESGE_SceneMngr::Quit();
	ESGE_QuitAudio();
	ESGE_Display::Quit();
	SDL_Quit();

	return 0;
}