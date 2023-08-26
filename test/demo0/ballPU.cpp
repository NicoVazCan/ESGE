#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_spritesheet.h"
#include "ESGE_anim.h"
#include "ESGE_time.h"

#include "ESGE_objCell.h"
#include "ESGE_objUpdate.h"
#include "ESGE_objDrawSprite.h"

#include "player.h"


static const ESGE_Frm _frms[] = {
  {0, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {3, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {4, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
};

static const ESGE_Anim _anim = {
  _frms,
  SDL_arraysize(_frms),
  -1
};


class ObjBallPU:
  public ESGE_ObjScene,
  public ESGE_ObjCell,
  public ESGE_ObjUpdate,
  public ESGE_ObjDrawSprite
{
  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;
  ObjPlayer *player;

public:
  static int GetCol(void *obj)
  {
    return ((ObjBallPU*)obj)->ESGE_ObjCell::GetCol();
  }
  static void SetCol(void *obj, int value)
  {
    ((ObjBallPU*)obj)->ESGE_ObjCell::SetCol(value);
  }

  static int GetRow(void *obj)
  {
    return ((ObjBallPU*)obj)->ESGE_ObjCell::GetRow();
  }
  static void SetRow(void *obj, int value)
  {
    ((ObjBallPU*)obj)->ESGE_ObjCell::SetRow(value);
  }

#define SS "sprites/ballPU.sprite.bin"

  ObjBallPU(void)
  {
    layer = PLAYER_LAYER-1;
    
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

    animPlayer.sprts = spritesheet;
    animPlayer.speed = 100;
    animPlayer.Start(&_anim);
    animPlayer.GetSprite(&sprite);
  }
  virtual ~ObjBallPU(void)
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  }

  virtual void OnStart(void) override
  {
    SDL_assert((player = ESGE_GetObj<ObjPlayer>("ObjPlayer")));
  }

  virtual void OnUpdate(void) override
  {
    SDL_Rect playerHitBox, triggerBox;

    playerHitBox = player->GetHitBox();

    triggerBox.x = pos.x;
    triggerBox.y = pos.y;
    triggerBox.w = cellW;
    triggerBox.h = cellH;

    if (SDL_HasIntersection(&playerHitBox, &triggerBox))
    {
      player->UnlockBall();
      Destroy();
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
  ObjBallPU,
  ESGE_FIELD(
    I,
    col,
    ObjBallPU::GetCol,
    ObjBallPU::SetCol,
    ESGE_FIELD(
      I,
      row,
      ObjBallPU::GetRow,
      ObjBallPU::SetRow,
      ESGE_END_FIELD
    )
  )
)