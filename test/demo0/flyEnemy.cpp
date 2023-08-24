#include "flyEnemy.h"

#include "ESGE_time.h"
#include "ESGE_file.h"
#include "ESGE_hash.h"

#include "player.h"

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
  {0, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*4},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*4},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*4},
  {3, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*4},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*4},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*4}
};

static const ESGE_Anim _anim = {
  _frms,
  SDL_arraysize(_frms),
  -1
};


#define POS_SCALE 8

int
ObjFlyEnemy::GetPosX(void *obj)
{
  return ((ObjFlyEnemy*)obj)->pos.x;
}
void
ObjFlyEnemy::SetPosX(void *obj, int value)
{
  ((ObjFlyEnemy*)obj)->pos.x = value;
  ((ObjFlyEnemy*)obj)->fPos.x = value << POS_SCALE;
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
  ((ObjFlyEnemy*)obj)->fPos.y = value << POS_SCALE;
}

#define SS "sprites/fly.sprite.bin"
#define MAX_LIFE 2

ObjFlyEnemy::ObjFlyEnemy(void)
{
  layer = PLAYER_LAYER+1;

  life = MAX_LIFE;

  offsetSize.x = 0;
  offsetSize.y = 0;
  offsetSize.w = 16;
  offsetSize.h = 8;

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
ObjFlyEnemy::OnStart(void)
{
  player = ESGE_GetObj<ObjPlayer>("scene0.bin", "ObjPlayer");
}

#define FOCUS_RANGE 128
#define ACC ( \
  ((int)(ESGE_deltaTm * ESGE_deltaTm)) * 0x0004 / 256 \
)
#define VEL (((int)ESGE_deltaTm) * 0x0080 / 16)
#define ROUND(T, S, N) ( \
  ((N + (1<<(S-1)) + (N>>(sizeof(T)*8-1))) & (~((1<<S)-1))) >> S \
)
#define DMG 10

void
ObjFlyEnemy::OnUpdate(void)
{
  if (dmgDeltaTm < maxDmgDeltaTm)
    dmgDeltaTm += ESGE_deltaTm;
  else
  {
    if (player)
    {
      SDL_Rect playerHitBox, atkBox;

      playerHitBox = player->GetHitBox();
      atkBox = GetHitBox();

      if (SDL_HasIntersection(&playerHitBox, &atkBox))
      {
        player->OnAttack(DMG);
      }

      if (
        SDL_abs(player->pos.x - pos.x) <= FOCUS_RANGE &&
        SDL_abs(player->pos.y - pos.y) <= FOCUS_RANGE
      )
      {
        if (player->pos.x > pos.x)
        {
          if (fVel.x + ACC >= VEL)
          {
            fAcc.x = 0;
            fVel.x = VEL;
          }
          else fAcc.x = ACC;
        }
        else if (player->pos.x < pos.x)
        {
          if (fVel.x - ACC <= -VEL)
          {
            fAcc.x = 0;
            fVel.x = -VEL;
          }
          else fAcc.x = -ACC;
        }

        if (player->pos.y > pos.y)
        {
          if (fVel.y + ACC >= VEL)
          {
            fAcc.y = 0;
            fVel.y = VEL;
          }
          else fAcc.y = ACC;
        }
        else if (player->pos.y < pos.y)
        {
          if (fVel.y - ACC <= -VEL)
          {
            fAcc.y = 0;
            fVel.y = -VEL;
          }
          else fAcc.y = -ACC;
        }
      }
      else
      {
        if (fVel.x > 0)
        {
          if (fVel.x - ACC <= 0)
          {
            fAcc.x = 0;
            fVel.x = 0;
          }
          else fAcc.x = -ACC;
        }
        else if (fVel.x < 0)
        {
          if (fVel.x + ACC >= 0)
          {
            fAcc.x = 0;
            fVel.x = 0;
          }
          else fAcc.x = ACC;
        }
        else
        {
          fAcc.x = 0;
          fVel.x = 0;
        }

        if (fVel.y > 0)
        {
          if (fVel.y - ACC <= 0)
          {
            fAcc.y = 0;
            fVel.y = 0;
          }
          else fAcc.y = -ACC;
        }
        else if (fVel.y < 0)
        {
          if (fVel.y + ACC >= 0)
          {
            fAcc.y = 0;
            fVel.y = 0;
          }
          else fAcc.y = ACC;
        }
        else
        {
          fAcc.y = 0;
          fVel.y = 0;
        }
      }
      
      fPos.x += fVel.x += fAcc.x;
      fPos.y += fVel.y += fAcc.y;

      pos.x = ROUND(int, POS_SCALE, fPos.x);
      pos.y = ROUND(int, POS_SCALE, fPos.y);
    }
  }

  animPlayer.Update(ESGE_deltaTm);
  animPlayer.GetSprite(&sprite);

  if (life == 0 && dmgDeltaTm >= maxDmgDeltaTm)
  {
    ESGE_Scene *scene;

    scene = ESGE_SceneMngr::GetActiveScene();

    if (scene->id == sceneID)
      scene->DelObj(instName);
    else
    {
      ESGE_Scene *prevScene = scene;

      ESGE_SceneMngr::SetActiveScene(sceneID);

      scene = ESGE_SceneMngr::GetActiveScene();
      scene->DelObj(instName);

      ESGE_SceneMngr::SetActiveScene(prevScene->id);
    }
  }
}

#define BLINK_T 16*4

void
ObjFlyEnemy::OnDraw(void)
{
  if (dmgDeltaTm >= maxDmgDeltaTm)
  {
    ESGE_ObjDrawSprite::OnDraw();
  }
  else
  {
    if (dmgDeltaTm % (BLINK_T*2) >= BLINK_T)
    {
      ESGE_ObjDrawSprite::OnDraw();
    }
  }
}

void
ObjFlyEnemy::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  EnableUpdate();
  EnableDraw();
  EnableEnemy();

  ESGE_ShareObj<ObjFlyEnemy>(this);
}
void
ObjFlyEnemy::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  DisableUpdate();
  DisableDraw();
  DisableEnemy();

  ESGE_UnshareObj<ObjFlyEnemy>(this);
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

void
ObjFlyEnemy::OnAttack(int dmg)
{
  if (dmgDeltaTm >= maxDmgDeltaTm)
  {
    dmgDeltaTm = 0;
    life -= dmg;
  }
}
