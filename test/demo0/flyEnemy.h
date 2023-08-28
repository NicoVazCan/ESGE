#ifndef FLY_ENEMY_H_
# define FLY_ENEMY_H_

#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_objUpdate.h"
#include "ESGE_anim.h"

#include "ESGE_objDrawSprite.h"
#include "enemy.h"

class ObjPlayer;
class ESGE_Spritesheet;
class ESGE_Sound;

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

  ESGE_Sound *dmgSnd, *deathSnd;

  Uint32 dmgDeltaTm;
  Uint32 maxDmgDeltaTm;

public:
  static int GetPosX(void *obj);
  static void SetPosX(void *obj, int value);

  static int GetPosY(void *obj);
  static void SetPosY(void *obj, int value);

  ObjFlyEnemy(void);
  virtual ~ObjFlyEnemy(void) override;

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