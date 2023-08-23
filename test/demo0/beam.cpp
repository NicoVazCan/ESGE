#include "beam.h"

#include "ESGE_time.h"
#include "ESGE_file.h"
#include "ESGE_hash.h"

#include "player.h"
#include "spawnerEnemy.h"
#include "alive.h"

ESGE_TYPE_FIELDS(
  ObjBeam,
  ESGE_FIELD(
    I,
    pos.x,
    ObjBeam::GetPosX,
    ObjBeam::SetPosX,
    ESGE_FIELD(
      I,
      pos.y,
      ObjBeam::GetPosY,
      ObjBeam::SetPosY,
      ESGE_FIELD(
        I,
        dir,
        ObjBeam::GetDir,
        ObjBeam::SetDir,
        ESGE_END_FIELD
      )
    )
  )
)

static const ESGE_Frm _frms[] = {
  {0, 0, 1., 0., {8, 4}, SDL_FLIP_NONE, 16*8},
  {1, 0, 1., 0., {8, 4}, SDL_FLIP_NONE, 16*8}
};

static const ESGE_Anim _anim = {
  _frms,
  SDL_arraysize(_frms),
  -1
};


#define POS_SCALE 8

int
ObjBeam::GetPosX(void *obj)
{
  return ((ObjBeam*)obj)->pos.x;
}
void
ObjBeam::SetPosX(void *obj, int value)
{
  ((ObjBeam*)obj)->pos.x = value;
  ((ObjBeam*)obj)->fPos.x = value << POS_SCALE;
}

int
ObjBeam::GetPosY(void *obj)
{
  return ((ObjBeam*)obj)->pos.y;
}
void
ObjBeam::SetPosY(void *obj, int value)
{
  ((ObjBeam*)obj)->pos.y = value;
  ((ObjBeam*)obj)->fPos.y = value << POS_SCALE;
}

#define VEL (((int)ESGE_deltaTm) * 0x0140 / 16)

int
ObjBeam::GetDir(void *obj)
{
  return ((ObjBeam*)obj)->dir;
}
void
ObjBeam::SetDir(void *obj, int value)
{
  switch (value)
  {
  case R:
    ((ObjBeam*)obj)->dir = R;
    ((ObjBeam*)obj)->sprite.angle = 0.;

    ((ObjBeam*)obj)->fVel.x = VEL;
    ((ObjBeam*)obj)->fVel.y = 0;
    break;
  case L:
    ((ObjBeam*)obj)->dir = L;
    ((ObjBeam*)obj)->sprite.angle = 180.;

    ((ObjBeam*)obj)->fVel.x = -VEL;
    ((ObjBeam*)obj)->fVel.y = 0;
    break;
  case D:
    ((ObjBeam*)obj)->dir = D;
    ((ObjBeam*)obj)->sprite.angle = 270.;

    ((ObjBeam*)obj)->fVel.x = 0;
    ((ObjBeam*)obj)->fVel.y = VEL;
    break;
  case U:
    ((ObjBeam*)obj)->dir = U;
    ((ObjBeam*)obj)->sprite.angle = 90.;

    ((ObjBeam*)obj)->fVel.x = 0;
    ((ObjBeam*)obj)->fVel.y = -VEL;
    break;
  default:
    ((ObjBeam*)obj)->dir = R;
    ((ObjBeam*)obj)->sprite.angle = 0.;

    ((ObjBeam*)obj)->fVel.x = VEL;
    ((ObjBeam*)obj)->fVel.y = 0;
  }
}

#define SS "sprites/beam.sprite.bin"

ObjBeam::ObjBeam(void)
{
  layer = PLAYER_LAYER-1;

  fVel.x = VEL;

  spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

  animPlayer.sprts = spritesheet;
  animPlayer.speed = 100;
  animPlayer.Start(&_anim);
  animPlayer.GetSprite(&sprite);
}
ObjBeam::~ObjBeam(void)
{
  ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
}

#define ROUND(T, S, N) ( \
  ((N + (1<<(S-1)) + (N>>(sizeof(T)*8-1))) & (~((1<<S)-1))) >> S \
)

void
ObjBeam::OnUpdate(void)
{
  fPos.x += fVel.x += fAcc.x;
  fPos.y += fVel.y += fAcc.y;

  pos.x = ROUND(int, POS_SCALE, fPos.x);
  pos.y = ROUND(int, POS_SCALE, fPos.y);

  animPlayer.Update(ESGE_deltaTm);
  animPlayer.GetSprite(&sprite);
}

void
ObjBeam::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  EnableUpdate();
  EnableDraw();
}
void
ObjBeam::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  DisableUpdate();
  DisableDraw();
}
#ifdef ESGE_EDITOR
void
ObjBeam::OnEditorInit(void)
{
  EnableDraw();
}
void
ObjBeam::OnEditorQuit(void)
{
  DisableDraw();
}
#endif
