#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_spritesheet.h"

#include "ESGE_objDrawSprite.h"

#define BGRND_SS "sprites/bgnd.sprite.bin"

#define PARLX_F_H 2
#define PARLX_F_V 2

class ObjParlxBgnd: public ESGE_ObjScene, public ESGE_ObjDrawSprite
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
    ESGE_ObjScene::OnEnable();
    EnableDraw();
  }
  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
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
    SDL_Point pos;

    pos.x = -ESGE_Display::cam.x / PARLX_F_H;
    pos.y = -ESGE_Display::cam.y / PARLX_F_V;

    ESGE_Display::DisplayDrawSprite(&sprite, pos);
  }
};

ESGE_TYPE(ObjParlxBgnd)
