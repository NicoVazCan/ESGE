#include <SDL2/SDL.h>

#define PLC_ASSERT SDL_assert
#define PLC_MALLOC SDL_malloc
#define PLC_FREE   SDL_free
#include <PLC.h>

#define PLAYER_LEFT  0x1
#define PLAYER_RIGHT 0x2
#define PLAYER_UP    0x4
#define PLAYER_DOWN  0x8
#define PLAYER_VEL   2
#define PLAYER_MOVE_PRI 0
#define PLAYER_DRAW_PRI PLAYER_MOVE_PRI+1

typedef struct Player
{
	SDL_Point kPos;
	int iKeys;
} Player;

Player* PlayerCreate(SDL_Point kPos);
void PlayerDestroy(Player *pkThis);
int PlayerEvent(void* pvThis, SDL_Event *pkEvent);
void PlayerMove(PLC_PriGrps **ppkPriGrps, void *pvThis);
void PlayerDraw(PLC_PriGrps **ppkPriGrps, void *pvThis);

// .c

SDL_Renderer *pkRend;
PLC_PriGrps *pkPriGrps;

Player*
PlayerCreate(SDL_Point kPos)
{
	Player *pkThis;

	pkThis = SDL_malloc(sizeof(Player));

	if (pkThis == NULL) return NULL;

	SDL_AddEventWatch(PlayerEvent, pkThis);
	PLC_add(&pkPriGrps, PLAYER_MOVE_PRI, PlayerMove, pkThis);
	PLC_add(&pkPriGrps, PLAYER_DRAW_PRI, PlayerDraw, pkThis);

	*pkThis = (Player){
		.kPos=kPos,
		.iKeys=0
	};

	return pkThis;
}

void
PlayerDestroy(Player *pkThis)
{
	SDL_DelEventWatch(PlayerEvent, pkThis);
	PLC_remove(&pkPriGrps, PLAYER_MOVE_PRI, PlayerMove, pkThis);
	PLC_remove(&pkPriGrps, PLAYER_DRAW_PRI, PlayerDraw, pkThis);

	SDL_zerop(pkThis);
	SDL_free(pkThis);
}

void*
PlayerLoad(SDL_RWops *pkIO)
{
	return PlayerCreate(
	 	(SDL_Point){
			.x = (Sint16)SDL_ReadBE16(pkIO),
			.y = (Sint16)SDL_ReadBE16(pkIO)
		}
	);
}

void
PlayerUnload(void *pvThis, SDL_RWops *pkIO)
{ 
	Player *pkThis = pvThis;

	SDL_WriteBE16(pkIO, (Sint16)pkThis->kPos.x);
	SDL_WriteBE16(pkIO, (Sint16)pkThis->kPos.y);

	PlayerDestroy(pkThis);
}

int
PlayerEvent(void *pvThis, SDL_Event *pkEvent)
{
	Player *pkThis = pvThis;

	switch (pkEvent->type)
	{
	case SDL_KEYDOWN:
			switch (pkEvent->key.keysym.sym)
			{
			case SDLK_LEFT:
				pkThis->iKeys |= PLAYER_LEFT;
				break;
			case SDLK_RIGHT:
				pkThis->iKeys |= PLAYER_RIGHT;
				break;
			case SDLK_UP:
				pkThis->iKeys |= PLAYER_UP;
				break;
			case SDLK_DOWN:
				pkThis->iKeys |= PLAYER_DOWN;
				break;
			default:
				break;
			}
		break;
	case SDL_KEYUP:
		switch (pkEvent->key.keysym.sym)
			{
			case SDLK_LEFT:
				pkThis->iKeys ^= PLAYER_LEFT;
				break;
			case SDLK_RIGHT:
				pkThis->iKeys ^= PLAYER_RIGHT;
				break;
			case SDLK_UP:
				pkThis->iKeys ^= PLAYER_UP;
				break;
			case SDLK_DOWN:
				pkThis->iKeys ^= PLAYER_DOWN;
				break;
			default:
				break;
			}
		break;
	default:
		break;
	}

	return 0;
}

void
PlayerMove(PLC_PriGrps **ppkPriGrps, void *pvThis)
{
	Player *pkThis = pvThis;

	if (pkThis->iKeys & PLAYER_LEFT)  pkThis->kPos.x -= PLAYER_VEL;
	if (pkThis->iKeys & PLAYER_RIGHT) pkThis->kPos.x += PLAYER_VEL;
	if (pkThis->iKeys & PLAYER_UP)    pkThis->kPos.y -= PLAYER_VEL;
	if (pkThis->iKeys & PLAYER_DOWN)  pkThis->kPos.y += PLAYER_VEL;
}

void
PlayerDraw(PLC_PriGrps **ppkPriGrps, void *pvThis)
{
	Player *pkThis = pvThis;

	SDL_Rect kPlayer = {
		.x=pkThis->kPos.x, .y=pkThis->kPos.y,
		.w=16, .h=16
	};

	SDL_SetRenderDrawColor(
		pkRend,
		0xFF,
		0x00,
		0x00,
		SDL_ALPHA_OPAQUE
	);
	SDL_RenderFillRect(pkRend, &kPlayer);
}


int ExitEvent(void *pvRun, SDL_Event *pkEvent)
{
	int *piRun = pvRun;

	*piRun = pkEvent->type != SDL_QUIT;

	return 0;
}


#define MSPF 33u

int
main(int argc, char const *argv[])
{
	SDL_Window *pkWindow;
	void *pvPlayer;
	Uint32 iTicks, iDelta = 0u;
	SDL_RWops *pkIO;
	int iRun = 1;

	PLC_init(&pkPriGrps);

	SDL_Init(SDL_INIT_VIDEO);

	pkWindow = SDL_CreateWindow(
		"Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		160,
		160,
		0
	);

	pkRend = SDL_CreateRenderer(pkWindow, -1, SDL_RENDERER_ACCELERATED);

	pkIO = SDL_RWFromFile("player.bin", "rb");
	pvPlayer = PlayerLoad(pkIO);
	SDL_RWclose(pkIO);
	pkIO = NULL;

	iTicks = SDL_GetTicks();

	SDL_AddEventWatch(ExitEvent, &iRun);

	while (iRun)
	{
		SDL_SetRenderDrawColor(
			pkRend,
			0x80,
			0x80,
			0x80,
			SDL_ALPHA_OPAQUE
		);
		SDL_RenderClear(pkRend);
		SDL_PumpEvents();
		PLC_update(&pkPriGrps);
		SDL_RenderPresent(pkRend);

		
		iTicks += iDelta = SDL_GetTicks() - iTicks;
    if (iDelta < MSPF) SDL_Delay(MSPF - iDelta);
	}

	pkIO = SDL_RWFromFile("player.bin", "wb");
	PlayerUnload(pvPlayer, pkIO);
	SDL_RWclose(pkIO);
	pkIO = NULL;

	SDL_DelEventWatch(ExitEvent, &iRun);

	SDL_DestroyRenderer(pkRend);
  SDL_DestroyWindow(pkWindow);

	SDL_Quit();
	return 0;
}
