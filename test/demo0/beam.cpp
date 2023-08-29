#include "beam.h"

#include "ESGE_time.h"
#include "ESGE_file.h"
#include "ESGE_hash.h"

#include "player.h"
#include "enemy.h"

ESGE_TYPE_FIELDS(
  ObjBeam,
  ESGE_FIELD(
    pos.x,
    ObjBeam::GetPosX,
    ObjBeam::SetPosX,
    ESGE_FIELD(
      pos.y,
      ObjBeam::GetPosY,
      ObjBeam::SetPosY,
      ESGE_FIELD(
        dir,
        ObjBeam::GetDir,
        ObjBeam::SetDir,
        ESGE_END_FIELD
      )
    )
  )
)

static const ESGE_Frm _frmsR[] = {
  {0, 0, 1., 0., {8, 4}, SDL_FLIP_NONE, 16*3},
  {1, 0, 1., 0., {8, 4}, SDL_FLIP_NONE, 16*3}
};

static const ESGE_Frm _frmsL[] = {
  {0, 0, 1., 180., {8, 4}, SDL_FLIP_NONE, 16*3},
  {1, 0, 1., 180., {8, 4}, SDL_FLIP_NONE, 16*3}
};

static const ESGE_Frm _frmsD[] = {
  {0, 0, 1., 90., {8, 4}, SDL_FLIP_NONE, 16*3},
  {1, 0, 1., 90., {8, 4}, SDL_FLIP_NONE, 16*3}
};

static const ESGE_Frm _frmsU[] = {
  {0, 0, 1., 270., {8, 4}, SDL_FLIP_NONE, 16*3},
  {1, 0, 1., 270., {8, 4}, SDL_FLIP_NONE, 16*3}
};

static const ESGE_Anim _anims[] = {
  {_frmsR, SDL_arraysize(_frmsR), -1},
  {_frmsL, SDL_arraysize(_frmsL), -1},
  {_frmsD, SDL_arraysize(_frmsD), -1},
  {_frmsU, SDL_arraysize(_frmsU), -1}
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
  ((ObjBeam*)obj)->prevPos.x = value;
  ((ObjBeam*)obj)->fPos.x = value << POS_SCALE;
  ((ObjBeam*)obj)->iPos.x = value;
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
  ((ObjBeam*)obj)->prevPos.y = value;
  ((ObjBeam*)obj)->fPos.y = value << POS_SCALE;
  ((ObjBeam*)obj)->iPos.y = value;
}

#define VEL (((int)ESGE_deltaTm) * 0x0400 / 16)

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
  case BEAM_R:
    ((ObjBeam*)obj)->dir = BEAM_R;

    ((ObjBeam*)obj)->fVel.x = VEL;
    ((ObjBeam*)obj)->fVel.y = 0;
    break;
  case BEAM_L:
    ((ObjBeam*)obj)->dir = BEAM_L;

    ((ObjBeam*)obj)->fVel.x = -VEL;
    ((ObjBeam*)obj)->fVel.y = 0;
    break;
  case BEAM_D:
    ((ObjBeam*)obj)->dir = BEAM_D;

    ((ObjBeam*)obj)->fVel.x = 0;
    ((ObjBeam*)obj)->fVel.y = VEL;
    break;
  case BEAM_U:
    ((ObjBeam*)obj)->dir = BEAM_U;

    ((ObjBeam*)obj)->fVel.x = 0;
    ((ObjBeam*)obj)->fVel.y = -VEL;
    break;
  default:
    SDL_assert(0);
  }
  ((ObjBeam*)obj)->animPlayer.Start(_anims + value);
  ((ObjBeam*)obj)->animPlayer.GetSprite(&((ObjBeam*)obj)->sprite);
}

#define SS "sprites/beam.sprite.bin"

ObjBeam::ObjBeam(void)
{
  layer = PLAYER_LAYER-1;

  fVel.x = VEL;

  spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

  animPlayer.sprts = spritesheet;
  animPlayer.speed = 100;
  animPlayer.Start(_anims + BEAM_R);
  animPlayer.GetSprite(&sprite);
}
ObjBeam::~ObjBeam(void)
{
  ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
}

#define ROUND(T, S, N) ( \
  ((N + (1<<(S-1)) + (N>>(sizeof(T)*8-1))) & (~((1<<S)-1))) >> S \
)

#define DMG 1
#define MAX_DIST 64

void
ObjBeam::OnUpdate(void)
{
  SDL_Rect atkBox;

  fPos.x += fVel.x += fAcc.x;
  fPos.y += fVel.y += fAcc.y;

  pos.x = ROUND(int, POS_SCALE, fPos.x);
  pos.y = ROUND(int, POS_SCALE, fPos.y);

  switch (dir)
  {
  case BEAM_R:
  case BEAM_L:
    atkBox.x = pos.x - 8;
    atkBox.y = pos.y - 4;
    atkBox.w = 16;
    atkBox.h = 8;
    break;
  case BEAM_D:
  case BEAM_U:
    atkBox.x = pos.x - 4;
    atkBox.y = pos.y - 8;
    atkBox.w = 8;
    atkBox.h = 16;
    break;
  default:
    SDL_assert(0);
  }

  ObjEnemy::Attack(atkBox, DMG);

  animPlayer.Update(ESGE_deltaTm);
  animPlayer.GetSprite(&sprite);

  if (
    SDL_abs(pos.x - iPos.x) >= MAX_DIST ||
    SDL_abs(pos.y - iPos.y) >= MAX_DIST
  )
    Destroy();
}

void
ObjBeam::OnCollideUp(SDL_UNUSED ESGE_ObjCollider *other)
{
  if (IsEnabled())
    Destroy();
}
void
ObjBeam::OnCollideDown(SDL_UNUSED ESGE_ObjCollider *other)
{
  if (IsEnabled())
    Destroy();
}
void
ObjBeam::OnCollideLeft(SDL_UNUSED ESGE_ObjCollider *other)
{
  if (IsEnabled())
    Destroy();
}
void
ObjBeam::OnCollideRight(SDL_UNUSED ESGE_ObjCollider *other)
{
  if (IsEnabled())
    Destroy();
}

void
ObjBeam::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  EnableUpdate();
  EnablePhysic();
  EnableDraw();
}
void
ObjBeam::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  DisableUpdate();
  DisablePhysic();
  DisableDraw();
}
#ifdef ESGE_EDITOR
void
ObjBeam::OnEditorEnable(void)
{
  EnableDraw();
}
void
ObjBeam::OnEditorDisable(void)
{
  DisableDraw();
}
#endif
