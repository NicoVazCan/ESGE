#include "ESGE_scene.h"

#include "ESGE_objEvent.h"

class ObjQuit:
  public ESGE_ObjScene,
  public ESGE_ObjKeyEvent,
  public ESGE_ObjJoyEvent
{
  virtual void OnEnable(void) override
  {
    ESGE_ObjScene::OnEnable();
    EnableKeyEvent();
    EnableJoyEvent();
  }
  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableKeyEvent();
    DisableJoyEvent();
  }
  
  virtual void OnKeyDown(
    SDL_Keycode key,
    SDL_UNUSED SDL_Keymod mod
  ) override
  {
    if (key == SDLK_ESCAPE)
      ESGE_quit = true;
  }

#define BUTTON_SELECT 6

  virtual void OnJoyButtonDown(Uint8 button)
  {
    if (button == BUTTON_SELECT)
      ESGE_quit = true;
  }
};

ESGE_TYPE(ObjQuit)
