#include <SDL2/SDL.h>
#include "ESGE_objSceneEditor.h"


int main(int argc, char const *argv[])
{
	SDL_assert(argc > 1);
	ESGE_ObjSceneEditor edirtor(argv[1]);

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	edirtor.RunCMD();

	SDL_Quit();

	return 0;
}