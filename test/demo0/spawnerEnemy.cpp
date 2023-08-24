#include "spawnerEnemy.h"

#include <SDL2/SDL.h>
#include "ESGE_time.h"
#include "ESGE_file.h"
#include "ESGE_hash.h"

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
#define MAX_LIFE 4

ObjSpawnerEnemy::ObjSpawnerEnemy(void)
{
  layer = PLAYER_LAYER-1;

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

void
ObjSpawnerEnemy::OnUpdate(void)
{
  if (spawnDeltaTm < maxSpawnDeltaTm)
    spawnDeltaTm += ESGE_deltaTm;

  if (dmgDeltaTm < maxDmgDeltaTm)
    dmgDeltaTm += ESGE_deltaTm;

  if (player)
  {
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
          SDL_assert(
            (
              fly = (ObjFlyEnemy*)ESGE_SceneMngr::GetActiveScene(
              )->AddObj("ObjFlyEnemy")
            )
          );

          ObjFlyEnemy::SetPosX(fly, pos.x + SPAWN_OFFSET_H);
          ObjFlyEnemy::SetPosY(fly, pos.y + SPAWN_OFFSET_V);
        }
      }
    }
  }

  animPlayer.Update(ESGE_deltaTm);
  animPlayer.GetSprite(&sprite);
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
    SDL_Log("hit %d dmg, %d life", dmg, life);
  }
}
