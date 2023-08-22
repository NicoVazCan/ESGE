#include "flyEnemy.h"

#include "ESGE_time.h"

ESGE_TYPE_FIELDS(
  ObjFlyEnemy,
  ESGE_FIELD(
    I,
    pos.x,
    ObjFlyEnemy::GetPosX,
    ObjFlyEnemy::SetPosX,
    ESGE_FIELD(
      I,
      pos.y,
      ObjFlyEnemy::GetPosY,
      ObjFlyEnemy::SetPosY,
      ESGE_END_FIELD
    )
  )
)

static const ESGE_Frm _frms[] = {
  {0, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*2},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*2},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*2},
  {3, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*2},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*2},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*2}
};

static const ESGE_Anim _anim = {
  _frms,
  SDL_arraysize(_frms),
  -1
};


#define SS "sprites/fly.sprite.bin"
#define POS_SCALE 8
#define ROUND(T, S, N) ( \
  ((N + (1<<(S-1)) + (N>>(sizeof(T)*8-1))) & (~((1<<S)-1))) >> S \
)

int
ObjFlyEnemy::GetPosX(void *obj)
{
  return ((ObjFlyEnemy*)obj)->pos.x;
}
void
ObjFlyEnemy::SetPosX(void *obj, int value)
{
  ((ObjFlyEnemy*)obj)->pos.x = value;
  ((ObjFlyEnemy*)obj)->prevPos.x = value;
}

int
ObjFlyEnemy::GetPosY(void *obj)
{
  return ((ObjFlyEnemy*)obj)->pos.y;
}
void
ObjFlyEnemy::SetPosY(void *obj, int value)
{
  ((ObjFlyEnemy*)obj)->pos.y = value;
  ((ObjFlyEnemy*)obj)->prevPos.y = value;
}


ObjFlyEnemy::ObjFlyEnemy(void)
{
  layer = 1;

  isTrigger = true;

  spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

  animPlayer.sprts = spritesheet;
  animPlayer.speed = 100;
  animPlayer.Start(&_anim);
  animPlayer.GetSprite(&sprite);
}
ObjFlyEnemy::~ObjFlyEnemy(void)
{
  ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
}

void
ObjFlyEnemy::OnInit(void)
{
  fPos.x = pos.x << POS_SCALE;
  fPos.y = pos.y << POS_SCALE;
}

void
ObjFlyEnemy::OnUpdate(void)
{
  pos.x = ROUND(int, POS_SCALE, fPos.x);
  pos.y = ROUND(int, POS_SCALE, fPos.y);

  animPlayer.Update(ESGE_deltaTm);
  animPlayer.GetSprite(&sprite);
}

void
ObjFlyEnemy::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  EnableUpdate();
  EnableDynamic();
  EnableDraw();
}
void
ObjFlyEnemy::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  DisableUpdate();
  DisableDynamic();
  DisableDraw();
}
#ifdef ESGE_EDITOR
void
ObjFlyEnemy::OnEditorInit(void)
{
  EnableDraw();
}
void
ObjFlyEnemy::OnEditorQuit(void)
{
  DisableDraw();
}
#endif
