#include <SDL2/SDL.h>
#include "ESGE_plc.h"
#include "ESGE_objEvent.h"

class TestQuit: ESGE_ObjQuitEvent
{
public:
	TestQuit(): ESGE_ObjQuitEvent() {SDL_Log("%s", SDL_FUNCTION);}
	~TestQuit() override {SDL_Log("%s", SDL_FUNCTION);};
protected:
	void OnQuit(void) override
	{
		SDL_Log("%s 0", SDL_FUNCTION);

		delete this;
	}
};

class TestQuitKey: ESGE_ObjQuitEvent, ESGE_ObjKeyEvent
{
public:
	TestQuitKey(
	): ESGE_ObjQuitEvent(), ESGE_ObjKeyEvent()
	{
		SDL_Log("%s", SDL_FUNCTION);
	}
	~TestQuitKey() override {SDL_Log("%s", SDL_FUNCTION);};
	void OnQuit(void) override
	{
		SDL_Log("%s 1", SDL_FUNCTION);

		delete this;
	}

	void OnKeyDown(SDL_Keycode key, SDL_Keymod mod) override
	{
		SDL_Log("pressed key=%u, mod=%u", key, mod);
	}

	void OnKeyUp(SDL_Keycode key, SDL_Keymod mod) override
	{
		SDL_Log("released key=%u, mod=%u", key, mod);
	}
};

int main(int argc, char const *argv[])
{
	SDL_Window *pWind;

	SDL_Init(SDL_INIT_VIDEO);

	pWind = SDL_CreateWindow(
    "Test",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    160,
    160,
    0
  );
	
  new TestQuit();
  new TestQuitKey();

  while (ESGE_AnyPLC())
	{
		ESGE_UpdatePLC();
		SDL_Delay(33);
	}

	SDL_DestroyWindow(pWind);

	SDL_Quit();

	return 0;
}