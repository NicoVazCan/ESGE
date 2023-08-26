#ifndef PLAYER_H_
# define PLAYER_H_

#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_anim.h"

#include "ESGE_objUpdate.h"
#include "ESGE_objDynamic.h"
#include "ESGE_objEvent.h"
#include "ESGE_objDrawSprite.h"
#include "alive.h"

#define PLAYER_LAYER 10

class ESGE_Spritesheet;
class ESGE_Sound;
class ObjRoomMngr;
class ObjCamMngr;
class ObjText;

class ObjPlayer:
  public ESGE_ObjScene,
  public ESGE_ObjKeyEvent,
  public ESGE_ObjUpdate,
  public ESGE_ObjDynamic,
  public ESGE_ObjDrawSprite,
  public ObjAlive
{
  SDL_Point fAcc = {0, 0};
  SDL_Point fVel = {0, 0};
  SDL_Point fPos;

  ObjRoomMngr *roomMngr;
  ObjCamMngr *camMngr;

  ESGE_ObjScene *uiText;

  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;
  ESGE_Sound *jmpSnd, *shotSnd, *dmgSnd;

  Uint32 dmgDeltaTm = 4000;
  Uint32 maxDmgDeltaTm = 4000;

  bool facingR, inGround, aimingUp;
  enum {STAND, RIGHT, LEFT} going;

  void Jump(void);
  void StopJump(void);

  void GoRight(void);
  void StopGoRight(void);

  void GoLeft(void);
  void StopGoLeft(void);

  void AimUp(void);
  void StopAimUp(void);

  void Shot(void);

  bool IsInGround(void);

  void UpdateLifeUI(void);

public:
  static int GetPosX(void *obj);
  static void SetPosX(void *obj, int value);

  static int GetPosY(void *obj);
  static void SetPosY(void *obj, int value);

  ObjPlayer(void);
  virtual ~ObjPlayer(void) override;

  virtual void OnInit(void) override;
  virtual void OnQuit(void) override;

  virtual void OnStart(void) override;

  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod);
  virtual void OnKeyUp(SDL_Keycode key, SDL_Keymod mod);

  virtual void OnUpdate(void) override;

  virtual void OnPhysic(void) override;

  virtual void OnCollide(ESGE_ObjCollider *other) override;

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