#include "ESGE_scene.h"

#include "ESGE_objEvent.h"

class ObjQuit: public ESGE_ObjScene, public ESGE_ObjKeyEvent
{
  virtual void OnEnable(void) override
  {
    ESGE_ObjScene::OnEnable();
    EnableKeyEvent();
  }
  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableKeyEvent();
  }
  
  virtual void OnKeyDown(
    SDL_Keycode key,
    SDL_UNUSED SDL_Keymod mod
  ) override
  {
    if (key == SDLK_ESCAPE)
      ESGE_quit = true;
  }
};

ESGE_TYPE(ObjQuit)
