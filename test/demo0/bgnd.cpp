#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_spritesheet.h"

#include "ESGE_objDrawSprite.h"

#define BGRND_SS "sprites/title.sprite.bin"

class ObjBgnd: public ESGE_ObjScene, public ESGE_ObjDrawSprite
{
  ESGE_Spritesheet *spritesheet;

public:
  virtual void OnInit(void) override
  {
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(BGRND_SS);
    spritesheet->GetSprite(&sprite, 0, 0);
  }
  virtual void OnQuit(void) override
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  }

  virtual void OnEnable(void) override
  {
    EnableDraw();
  }
  virtual void OnDisable(void) override
  {
    DisableDraw();
  }

#ifdef ESGE_EDITOR
  virtual void OnEditorInit(void) override
  {
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(BGRND_SS);
    spritesheet->GetSprite(&sprite, 0, 0);
    EnableDraw();
  }
  virtual void OnEditorQuit(void) override
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
    DisableDraw();
  }
#endif
  
  virtual void OnDraw(void) override
  {
    SDL_Point pos = {0, 0};

    ESGE_Display::DisplayDrawSprite(&sprite, pos);
  }
};

ESGE_TYPE(ObjBgnd)
