#ifndef SPAWNER_ENEMY_H_
# define SPAWNER_ENEMY_H_

#include "ESGE_scene.h"
#include "ESGE_objUpdate.h"
#include "ESGE_anim.h"

#include "ESGE_objCell.h"
#include "ESGE_objDrawSprite.h"
#include "enemy.h"

class ObjPlayer;
class ESGE_Spritesheet;
class ESGE_Sound;

class ObjSpawnerEnemy:
  public ESGE_ObjScene,
  public ESGE_ObjCell,
  public ESGE_ObjUpdate,
  public ESGE_ObjDrawSprite,
  public ObjEnemy
{
  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;

  Uint32 spawnDeltaTm;
  Uint32 maxSpawnDeltaTm;
  int maxFlyEnemy = 32;

  ESGE_Sound *dmgSnd, *deathSnd;

  Uint32 dmgDeltaTm;
  Uint32 maxDmgDeltaTm;

public:
  static int GetCol(void *obj);
  static void SetCol(void *obj, int value);

  static int GetRow(void *obj);
  static void SetRow(void *obj, int value);

  static int GetMaxFlyEnemy(void *obj);
  static void SetMaxFlyEnemy(void *obj, int value);

  ObjSpawnerEnemy(void);
  virtual ~ObjSpawnerEnemy(void);

  virtual void OnUpdate(void) override;

  virtual void OnDraw(void) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
#ifdef ESGE_EDITOR
  virtual void OnEditorEnable(void) override;
  virtual void OnEditorDisable(void) override;
#endif

  virtual void OnAttack(int dmg) override;
};

#endif