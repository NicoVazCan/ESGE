#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_spritesheet.h"
#include "ESGE_anim.h"
#include "ESGE_time.h"

#include "ESGE_objUpdate.h"
#include "ESGE_objDrawSprite.h"

#include "player.h"


static const ESGE_Frm _frms[] = {
  {0, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {3, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8}
};

static const ESGE_Anim _anim = {
  _frms,
  SDL_arraysize(_frms),
  -1
};


class ObjHeal:
  public ESGE_ObjScene,
  public ESGE_ObjUpdate,
  public ESGE_ObjDrawSprite
{
  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;

  Uint32 existingTm;

public:
  static int GetPosX(void *obj)
  {
    return ((ObjHeal*)obj)->pos.x;
  }
  static void SetPosX(void *obj, int value)
  {
    ((ObjHeal*)obj)->pos.x = value;
  }

  static int GetPosY(void *obj)
  {
    return ((ObjHeal*)obj)->pos.y;
  }
  static void SetPosY(void *obj, int value)
  {
    ((ObjHeal*)obj)->pos.y = value;
  }

#define SS "sprites/heal.sprite.bin"

  ObjHeal(void)
  {
    layer = PLAYER_LAYER-1;
    
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

    animPlayer.sprts = spritesheet;
    animPlayer.speed = 100;
    animPlayer.Start(&_anim);
    animPlayer.GetSprite(&sprite);

    existingTm = 0;
  }
  virtual ~ObjHeal(void)
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  }

#define HEAL 5
#define W 8
#define H 8
#define MAX_TM 5000

  virtual void OnUpdate(void) override
  {
    if (existingTm >= MAX_TM)
    {
      Destroy();
    }
    else
    {
      ObjPlayer *player;

      existingTm += ESGE_deltaTm;

      if ((player = ESGE_GetObj<ObjPlayer>("ObjPlayer")))
      {
        SDL_Rect playerHitBox, triggerBox;

        playerHitBox = player->ESGE_ObjDynamic::GetBox();

        triggerBox.x = pos.x;
        triggerBox.y = pos.y;
        triggerBox.w = W;
        triggerBox.h = H;

        if (SDL_HasIntersection(&playerHitBox, &triggerBox))
        {
          player->Heal(HEAL);
          Destroy();
        }
      }
    }

    animPlayer.Update(ESGE_deltaTm);
    animPlayer.GetSprite(&sprite);
  }

  virtual void OnEnable(void) override
  {
    ESGE_ObjScene::OnEnable();
    EnableUpdate();
    EnableDraw();
  }
  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableUpdate();
    DisableDraw();
  }
#ifdef ESGE_EDITOR
  virtual void OnEditorEnable(void) override
  {
    EnableDraw();
  }
  virtual void OnEditorDisable(void) override
  {
    DisableDraw();
  }
#endif
};

ESGE_TYPE_FIELDS(
  ObjHeal,
  ESGE_FIELD(
    I,
    pos.x,
    ObjHeal::GetPosX,
    ObjHeal::SetPosX,
    ESGE_FIELD(
      I,
      pos.y,
      ObjHeal::GetPosY,
      ObjHeal::SetPosY,
      ESGE_END_FIELD
    )
  )
)