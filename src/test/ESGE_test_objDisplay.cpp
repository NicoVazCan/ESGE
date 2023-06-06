#include <SDL2/SDL.h>
#include "ESGE_plc.h"
#include "ESGE_objEvent.h"
#include "ESGE_objDisplay.h"

class TestQuit: ESGE_ObjQuitEvent
{
public:
	TestQuit(): ESGE_ObjQuitEvent() {SDL_Log("%s", SDL_FUNCTION);}
	~TestQuit() override {SDL_Log("%s", SDL_FUNCTION);};
protected:
	void OnQuit(void) override
	{
		SDL_Log("%s", SDL_FUNCTION);

		delete this;
	}
};

int main(SDL_UNUSED int argc, SDL_UNUSED char const *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	ESGE_ObjDisplay display;

	SDL_SetRenderDrawColor(display.rend, 255, 0, 0, 255);
	

	new TestQuit();

	while (ESGE_AnyPLC())
	{
		ESGE_UpdatePLC();
		SDL_RenderClear(display.rend);
		SDL_RenderPresent(display.rend);
		SDL_Delay(33);
	}

	SDL_Quit();

	return 0;
}