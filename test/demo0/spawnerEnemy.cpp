#include "spawnerEnemy.h"

#include <SDL2/SDL.h>
#include "ESGE_time.h"
#include "ESGE_file.h"
#include "ESGE_hash.h"
#include "ESGE_error.h"

#include "flyEnemy.h"
#include "player.h"

ESGE_TYPE_FIELDS(
  ObjSpawnerEnemy,
  ESGE_FIELD(
    I,
    col,
    ObjSpawnerEnemy::GetCol,
    ObjSpawnerEnemy::SetCol,
    ESGE_FIELD(
      I,
      row,
      ObjSpawnerEnemy::GetRow,
      ObjSpawnerEnemy::SetRow,
      ESGE_FIELD(
        I,
        maxFlyEnemy,
        ObjSpawnerEnemy::GetMaxFlyEnemy,
        ObjSpawnerEnemy::SetMaxFlyEnemy,
        ESGE_END_FIELD
      )
    )
  )
)

static const ESGE_Frm _frms[] = {
  {0, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {3, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*20}
};

static const ESGE_Anim _anim = {_frms, SDL_arraysize(_frms), -1};


int
ObjSpawnerEnemy::GetCol(void *obj)
{
  return ((ObjSpawnerEnemy*)obj)->ESGE_ObjCell::GetCol();
}
void
ObjSpawnerEnemy::SetCol(void *obj, int value)
{
  ((ObjSpawnerEnemy*)obj)->ESGE_ObjCell::SetCol(value);
}

int
ObjSpawnerEnemy::GetRow(void *obj)
{
  return ((ObjSpawnerEnemy*)obj)->ESGE_ObjCell::GetRow();
}
void
ObjSpawnerEnemy::SetRow(void *obj, int value)
{
  ((ObjSpawnerEnemy*)obj)->ESGE_ObjCell::SetRow(value);
}

int
ObjSpawnerEnemy::GetMaxFlyEnemy(void *obj)
{
  return ((ObjSpawnerEnemy*)obj)->maxFlyEnemy;
}
void
ObjSpawnerEnemy::SetMaxFlyEnemy(void *obj, int value)
{
  ((ObjSpawnerEnemy*)obj)->maxFlyEnemy = value;
}


#define SS "sprites/spawner.sprite.bin"
#define MAX_LIFE 8

ObjSpawnerEnemy::ObjSpawnerEnemy(void)
{
  layer = PLAYER_LAYER-2;

  offsetSize.x = 0;
  offsetSize.y = 0;
  offsetSize.w = 32;
  offsetSize.h = 64;

  spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

  animPlayer.sprts = spritesheet;
  animPlayer.speed = 100;
  animPlayer.Start(&_anim);
  animPlayer.GetSprite(&sprite);

  life = MAX_LIFE;
}
ObjSpawnerEnemy::~ObjSpawnerEnemy(void)
{
  ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
}

void
ObjSpawnerEnemy::OnStart(void)
{
  player = ESGE_GetObj<ObjPlayer>("scene0.bin", "ObjPlayer");
}

#define SPAWN_OFFSET_H 16
#define SPAWN_OFFSET_V 32
#define FOCUS_RANGE 64
#define DMG 10

void
ObjSpawnerEnemy::OnUpdate(void)
{
  if (spawnDeltaTm < maxSpawnDeltaTm)
    spawnDeltaTm += ESGE_deltaTm;

  if (dmgDeltaTm < maxDmgDeltaTm)
    dmgDeltaTm += ESGE_deltaTm;

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
      if (spawnDeltaTm >= maxSpawnDeltaTm)
      {
        ObjFlyEnemy *fly;
        int nFly = 0;

        for (
          ESGE_ObjScene *obj = ESGE_GetSharedList<ObjFlyEnemy>();
          obj;
          obj = obj->nextShared
        )
          nFly++;

        if (nFly < maxFlyEnemy)
        {
          spawnDeltaTm = 0;
          if (
            (
              fly = (ObjFlyEnemy*)ESGE_SceneMngr::GetActiveScene(
              )->AddObj("ObjFlyEnemy")
            )
          )
          {
            ObjFlyEnemy::SetPosX(fly, pos.x + SPAWN_OFFSET_H);
            ObjFlyEnemy::SetPosY(fly, pos.y + SPAWN_OFFSET_V);
          }
          else
            SDL_Log("Failed to add ObjFlyEnemy: %s", SDL_GetError());
        }
      }
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
ObjSpawnerEnemy::OnDraw(void)
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
ObjSpawnerEnemy::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  EnableUpdate();
  EnableDraw();
  EnableEnemy();
}
void
ObjSpawnerEnemy::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  DisableUpdate();
  DisableDraw();
  DisableEnemy();
}
#ifdef ESGE_EDITOR
void
ObjSpawnerEnemy::OnEditorInit(void)
{
  EnableDraw();
}
void
ObjSpawnerEnemy::OnEditorQuit(void)
{
  DisableDraw();
}
#endif

void
ObjSpawnerEnemy::OnAttack(int dmg)
{
  if (dmgDeltaTm >= maxDmgDeltaTm)
  {
    dmgDeltaTm = 0;
    life -= dmg;
  }
}
