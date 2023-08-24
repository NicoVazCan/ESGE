#ifndef FLY_ENEMY_H_
# define FLY_ENEMY_H_

#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_objUpdate.h"
#include "ESGE_anim.h"

#include "ESGE_objDrawSprite.h"
#include "enemy.h"

class ObjPlayer;

class ObjFlyEnemy:
  public ESGE_ObjScene,
  public ESGE_ObjUpdate,
  public ESGE_ObjDrawSprite,
  public ObjEnemy
{
  SDL_Point fAcc = {0, 0};
  SDL_Point fVel = {0, 0};
  SDL_Point fPos;

  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;

  ObjPlayer *player;

  Uint32 dmgDeltaTm = 1000;
  Uint32 maxDmgDeltaTm = 1000;

public:
  static int GetPosX(void *obj);
  static void SetPosX(void *obj, int value);

  static int GetPosY(void *obj);
  static void SetPosY(void *obj, int value);

  ObjFlyEnemy(void);
  virtual ~ObjFlyEnemy(void) override;

  virtual void OnStart(void) override;

  virtual void OnUpdate(void) override;

  virtual void OnDraw(void) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
#ifdef ESGE_EDITOR
  virtual void OnEditorInit(void) override;
  virtual void OnEditorQuit(void) override;
#endif

  virtual void OnAttack(int dmg) override;
};

#endif