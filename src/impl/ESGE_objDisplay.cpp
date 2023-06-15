#include "ESGE_objDisplay.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

#define TITLE "test"
#define X SDL_WINDOWPOS_UNDEFINED
#define Y SDL_WINDOWPOS_UNDEFINED
#define W 256
#define H 144
#define WIND_FLAGS SDL_WINDOW_RESIZABLE
#define REND_INDEX -1
#define REND_FLAGS SDL_RENDERER_ACCELERATED

static ESGE_ObjSerial*
ESGE_LoadObjDisplay(SDL_RWops *io)
{
  Uint16 id;
  Uint8 full, vsync;
  ESGE_ObjDisplay *obj;

  if(!(id = SDL_ReadBE16(io))) return NULL;
  full = SDL_ReadU8(io);
  vsync = SDL_ReadU8(io);

  for (
    obj = ESGE_ObjDisplay::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjDisplay(id, full, vsync);
  }
  else return NULL;
}

static ESGE_Loader ESGE_ObjDisplayLoader(
  ESGE_OBJ_DISPLAY_TYPE_ID,
  ESGE_LoadObjDisplay
);

static ESGE_ObjSerial*
ESGE_TextLoadObjDisplay(void)
{
  char buff[8];
  Uint16 id;
  Uint8 full, vsync;
  ESGE_ObjDisplay *obj;

  while (
    !(fputs("id: ", stdout), fgets(buff, 7, stdin)) ||
    !(id = (unsigned)SDL_atoi(buff))
  );
  while(!(fputs("full: ", stdout), fgets(buff, 7, stdin)));
  full = (unsigned)SDL_atoi(buff);
  while(!(fputs("vsync: ", stdout), fgets(buff, 7, stdin)));
  vsync = (unsigned)SDL_atoi(buff);

  for (
    obj = ESGE_ObjDisplay::list;
    obj != NULL && obj->id < id;
    obj = obj->next
  );
  if (obj == NULL || obj->id != id)
  {
    return new ESGE_ObjDisplay(id, full, vsync);
  }
  else return NULL;
}

static ESGE_TextLoader ESGE_ObjDisplayTextLoader(
  ESGE_OBJ_DISPLAY_TYPE_ID,
  ESGE_TextLoadObjDisplay
);

ESGE_ObjDisplay *ESGE_ObjDisplay::list = NULL;

ESGE_ObjDisplay::ESGE_ObjDisplay(
  Uint16 id,
  Uint8 full,
  Uint8 vsync
):
  ESGE_ObjInScene(id),
  vsync(vsync)
{
  SDL_Window *wind;

  SDL_assert(
    (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) ||
    "Error: video was not initialized" == NULL
  );

  if (
    (
      wind = SDL_CreateWindow(
        TITLE,
        X,
        Y,
        W,
        H,
        full? WIND_FLAGS | SDL_WINDOW_FULLSCREEN: WIND_FLAGS
      )
    ) == NULL
  )
  {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", SDL_GetError());
    SDL_assert("Error while creating window" == NULL);
  }
  windowID = SDL_GetWindowID(wind);
  if (
    (rend = SDL_CreateRenderer(wind, REND_INDEX, REND_FLAGS)) == NULL
  )
  {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
    SDL_assert("Error while creating window renderer" == NULL);
  }
  if (SDL_RenderSetLogicalSize(rend, W, W))
  {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
    SDL_assert("Error while setting reder logical size" == NULL);
  }
  if (SDL_RenderSetVSync(rend, vsync))
  {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
    SDL_ClearError();
  }

  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjDisplay,
    list,
    this,
    ESGE_CMP_OBJ_SERIAL,
    next
  );
}

ESGE_ObjDisplay::~ESGE_ObjDisplay(void)
{
  SDL_Window *wind;

  wind = SDL_RenderGetWindow(rend);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);

  SGLIB_SORTED_LIST_DELETE(
    ESGE_ObjDisplay,
    list,
    this,
    next
  );
}

void
ESGE_ObjDisplay::OnKeyDown(SDL_Keycode key, SDL_Keymod mod)
{
  if (key == SDLK_F11 || (SDLK_RETURN == key && mod & KMOD_ALT))
  {
    SDL_Window *wind;

    wind = SDL_RenderGetWindow(rend);
    if (
      SDL_SetWindowFullscreen(
        wind,
        SDL_GetWindowFlags(wind) ^ SDL_WINDOW_FULLSCREEN
      )
    )
    {
      SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", SDL_GetError());
      SDL_ClearError();
    }
  }
  else if (SDLK_BACKSPACE == key && mod & KMOD_ALT)
  {
    if (SDL_RenderSetVSync(rend, vsync = ~vsync))
    {
      SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
      SDL_ClearError();
    }
  }
}

void
ESGE_ObjDisplay::OnSave(SDL_RWops *io) const
{
  Uint8 full;
  
  ESGE_ObjInScene::OnSave(io);

  full = (
    SDL_GetWindowFlags(SDL_RenderGetWindow(rend)) &
    SDL_WINDOW_FULLSCREEN
  ) > 0;

  if (!SDL_WriteU8(io, full))
  {
    SDL_LogError(
      SDL_LOG_CATEGORY_SYSTEM,
      "Error %s:%s:%d: %s",
      SDL_FILE,
      SDL_FUNCTION,
      SDL_LINE,
      SDL_GetError()
    );
    SDL_ClearError();
    SDL_assert("Failed to write ObjDisplay fullscreen" == NULL);
  }
  if (!SDL_WriteU8(io, vsync))
  {
    SDL_LogError(
      SDL_LOG_CATEGORY_SYSTEM,
      "Error %s:%s:%d: %s",
      SDL_FILE,
      SDL_FUNCTION,
      SDL_LINE,
      SDL_GetError()
    );
    SDL_ClearError();
    SDL_assert("Failed to write ObjDisplay vsync" == NULL);
  }
}

Uint16
ESGE_ObjDisplay::GetTypeID(void) const
{
  return ESGE_OBJ_DISPLAY_TYPE_ID;
}

void
ESGE_ObjDisplay::OnEnable(void)
{
  ESGE_ObjInScene::OnEnable();
  ESGE_ObjKeyEvent::OnEnable();
}

void
ESGE_ObjDisplay::OnDisable(void)
{
  ESGE_ObjInScene::OnDisable();
  ESGE_ObjKeyEvent::OnDisable();
}
