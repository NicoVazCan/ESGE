#include <SDL2/SDL.h>
#include "ESGE_plc.h"
#include "ESGE_objEvent.h"
#include "ESGE_objDisplay.h"

class TestQuit: public ESGE_ObjQuitEvent
{
public:
	ESGE_ObjDisplay *display;

	TestQuit()
	{
		const Uint8 data[4] = {0x00, 0x02, 0x00, 0xFF};
		SDL_RWops *io;

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);


		SDL_Log("%X %X  %X %X", data[0], data[1], data[2], data[3]);

		SDL_assert((io = SDL_RWFromConstMem((void*)data, 4)) != NULL);
		display = (ESGE_ObjDisplay*)ESGE_Loader::Load(2, io);
		SDL_RWclose(io);


		OnEnable();
	}
	~TestQuit() override
	{
		Uint8 data[4];
		SDL_RWops *io;

		OnDisable();


		SDL_assert((io = SDL_RWFromMem((void*)data, 4)) != NULL);
		display->OnSave(io);
		delete display;
		SDL_RWclose(io);

		SDL_Log("%X %X  %X %X", data[0], data[1], data[2], data[3]);


		SDL_Quit();
	}
	void OnEnable(void)
	{
		display->OnEnable();
		ESGE_ObjQuitEvent::OnEnable();
	}
	void OnDisable(void)
	{
		display->OnDisable();
		ESGE_ObjQuitEvent::OnDisable();
	}
protected:
	void OnQuit(void) override
	{
		delete this;
	}
};

int main(SDL_UNUSED int argc, SDL_UNUSED char const *argv[])
{
	TestQuit *test = new TestQuit();

	SDL_SetRenderDrawColor(test->display->rend, 255, 0, 0, 255);
	while (ESGE_AnyPLC())
	{
		ESGE_UpdatePLC();
		SDL_RenderClear(test->display->rend);
		SDL_RenderPresent(test->display->rend);
		SDL_Delay(33);
	}

	return 0;
}