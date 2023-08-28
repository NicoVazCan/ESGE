#include "spawnerEnemy.h"

#include <SDL2/SDL.h>
#include "ESGE_time.h"
#include "ESGE_file.h"
#include "ESGE_hash.h"
#include "ESGE_audio.h"

#include "flyEnemy.h"
#include "player.h"

#include <stdlib.h> //rand()

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
  {0, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {3, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*8}
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
#define DMG_SND "sounds/enemy_dmg.wav"
#define DEATH_SND "sounds/enemy_death.wav"
#define W 32
#define H 64
#define SPAWN_TM 1000
#define DMG_TM 500

ObjSpawnerEnemy::ObjSpawnerEnemy(void)
{
  layer = PLAYER_LAYER-2;

  ObjAlive::offsetSize.x = 0;
  ObjAlive::offsetSize.y = 0;
  ObjAlive::offsetSize.w = W;
  ObjAlive::offsetSize.h = H;

  spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

  dmgSnd = ESGE_FileMngr<ESGE_Sound>::Watch(DMG_SND);
  deathSnd = ESGE_FileMngr<ESGE_Sound>::Watch(DEATH_SND);

  animPlayer.sprts = spritesheet;
  animPlayer.speed = 100;
  animPlayer.Start(&_anim);
  animPlayer.GetSprite(&sprite);

  life = MAX_LIFE;

  spawnDeltaTm = maxSpawnDeltaTm = SPAWN_TM;
  dmgDeltaTm = maxDmgDeltaTm = DMG_TM;
}
ObjSpawnerEnemy::~ObjSpawnerEnemy(void)
{
  ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);

  ESGE_FileMngr<ESGE_Sound>::Leave(dmgSnd);
  ESGE_FileMngr<ESGE_Sound>::Leave(deathSnd);
}

#define SPAWN_OFFSET_H 16
#define SPAWN_OFFSET_V 32
#define FOCUS_RANGE 80
#define DMG 5
#define HEAL_PROB 50

void
ObjSpawnerEnemy::OnUpdate(void)
{
  if (spawnDeltaTm < maxSpawnDeltaTm)
    spawnDeltaTm += ESGE_deltaTm;

  if (dmgDeltaTm < maxDmgDeltaTm)
    dmgDeltaTm += ESGE_deltaTm;
  else
  {
    if (life <= 0)
    {
      if (rand() % 100 > HEAL_PROB)
      {
        ESGE_ObjScene *obj;
        const ESGE_Type *heal;
        const ESGE_Field *posX, *posY;
        SDL_Point center;

        center.x = pos.x + W/2;
        center.y = pos.y + H/2;

        obj = Create("ObjHeal");

        heal = ESGE_Type::Get(obj->typeID);

        posX = heal->GetField("pos.x");
        posY = heal->GetField("pos.y");

        posX->SetValue(obj, (void*)&center.x);
        posY->SetValue(obj, (void*)&center.y);
      }
      Destroy();
    }
    else
    {
      ObjPlayer *player;

      if ((player = ESGE_GetObj<ObjPlayer>("ObjPlayer")))
      {
        SDL_Rect playerHitBox, atkBox;

        playerHitBox = player->ObjAlive::GetBox();
        atkBox = ObjAlive::GetBox();

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
            ESGE_ObjScene *fly;
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
              fly = Create("ObjFlyEnemy");
              ObjFlyEnemy::SetPosX(fly, pos.x + SPAWN_OFFSET_H);
              ObjFlyEnemy::SetPosY(fly, pos.y + SPAWN_OFFSET_V);
            }
          }
        }
      }
    }
  }

  animPlayer.Update(ESGE_deltaTm);
  animPlayer.GetSprite(&sprite);
}

#define BLINK_T 16*2

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
ObjSpawnerEnemy::OnEditorEnable(void)
{
  EnableDraw();
}
void
ObjSpawnerEnemy::OnEditorDisable(void)
{
  DisableDraw();
}
#endif

void
ObjSpawnerEnemy::OnAttack(int dmg)
{
  if (life > 0 && dmgDeltaTm >= maxDmgDeltaTm)
  {
    dmgDeltaTm = 0;
    life -= dmg;

    if (life > 0)
      dmgSnd->Play();
    else
      deathSnd->Play();
  }
}
