#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_audio.h"
#include "ESGE_error.h"

#include "ESGE_objEvent.h"
#include "ESGE_objUpdate.h"
#include "ESGE_objPhysic.h"
#include "ESGE_objDraw.h"


Uint32 ESGE_deltaTm = 16, ESGE_realDeltaTm;

#define GAME_HELP \
"USAGE: game [OPTION]... [FILE]\n" \
"Opens a window with the scene from FILE to start the game.\n" \
"If no FILE specified, the scene is opened from the file named\n" \
"\"scene.bin\" in the current directory, if it does not exist an\n" \
"empty one is created.\n" \
"\n" \
"  -t Set window title, \"game\" by default\n" \
"  -W Set window width in pixels, 256 by default\n" \
"  -H Set window width in pixels, 144 by default\n" \
"  -f Set game loop frame per secound, 60 by default\n" \
"  -d Set maximum disabled scenes, 4 by default\n" \
"  -s Set sound volume, range 0 to 256, 15 by default\n" \
"  -m Set music volume, range 0 to 256, 15 by default\n" \
"  -h Print this mensage\n"

#ifdef __cplusplus
extern "C"
#endif
int
main(int argc, char *argv[])
{
	const char *title = "game", *sceneFile = "scene.bin";
	static const char *const help = GAME_HELP;
	int w = 256, h = 144, nDisabledScene = 4;
	Uint8 soundVolume = 0x0F, musicVolume = 0x0F;
	Uint32 ticks;

	for (int i = 1; i < argc; ++i)
	{
		if ('-' == argv[i][0])
		{
			if (i == argc-1)
			{
				puts(help);
				return 0;
			}
			switch (argv[i][1])
			{
			case 't':
				title = argv[++i];
				break;
			case 'W':
				SDL_sscanf(argv[++i], "%d", &w);
				break;
			case 'H':
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
			case 'h':
			default:
				puts(help);
				return 0;
			}
		}
		else sceneFile = argv[i];
	}

	if (
		SDL_Init(
			SDL_INIT_EVENTS |
			SDL_INIT_VIDEO |
			SDL_INIT_AUDIO |
			SDL_INIT_JOYSTICK
		)
	)
		ESGE_Error("Failed to initializa SDL: %s", SDL_GetError());
	ESGE_Display::Init(title, w, h);
	ESGE_InitAudio(soundVolume, musicVolume);
	ESGE_SceneMngr::Init(nDisabledScene);
	ESGE_SceneMngr::AddScene(sceneFile);

	for (;;)
	{
		ticks = SDL_GetTicks();

		ESGE_EventLoop();
		if (ESGE_quit) break;

		ESGE_ObjUpdate::Update();
		ESGE_ObjPhysic::Physic();
		ESGE_SceneMngr::Update();
		ESGE_ObjDraw::Draw();

		ESGE_Display::Update();

		ESGE_realDeltaTm = SDL_GetTicks() - ticks;

  	if (ESGE_realDeltaTm < ESGE_deltaTm)
  		SDL_Delay(ESGE_deltaTm - ESGE_realDeltaTm);
	}

	ESGE_SceneMngr::Quit();
	ESGE_QuitAudio();
	ESGE_Display::Quit();
	SDL_Quit();

	return 0;
}