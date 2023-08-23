#ifndef SPAWNER_ENEMY_H_
# define SPAWNER_ENEMY_H_

#include "ESGE_scene.h"
#include "ESGE_objUpdate.h"
#include "ESGE_anim.h"

#include "ESGE_objCell.h"
#include "ESGE_objDrawSprite.h"
#include "alive.h"

class ObjPlayer;

class ObjSpawnerEnemy:
  public ESGE_ObjScene,
  public ESGE_ObjCell,
  public ESGE_ObjUpdate,
  public ObjAlive,
  public ESGE_ObjDrawSprite
{
  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;

  Uint32 spawnDeltaTm = 2000;
  Uint32 maxSpawnDeltaTm = 2000;
  int maxFlyEnemy = 8;

  ObjPlayer *player;

public:
  static int GetCol(void *obj);
  static void SetCol(void *obj, int value);

  static int GetRow(void *obj);
  static void SetRow(void *obj, int value);

  static int GetMaxFlyEnemy(void *obj);
  static void SetMaxFlyEnemy(void *obj, int value);

  ObjSpawnerEnemy(void);
  virtual ~ObjSpawnerEnemy(void);

  virtual void OnStart(void) override;

  virtual void OnUpdate(void) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
#ifdef ESGE_EDITOR
  virtual void OnEditorInit(void) override;
  virtual void OnEditorQuit(void) override;
#endif
};

#endif