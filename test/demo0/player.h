#ifndef PLAYER_H_
# define PLAYER_H_

#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_anim.h"

#include "ESGE_objUpdate.h"
#include "ESGE_objDynamic.h"

#include "ESGE_objDrawSprite.h"
#include "alive.h"

#define PLAYER_LAYER 10

class ESGE_Spritesheet;
class ESGE_Sound;

class ObjPlayer:
  public ESGE_ObjScene,
  public ESGE_ObjUpdate,
  public ESGE_ObjDynamic,
  public ESGE_ObjDrawSprite,
  public ObjAlive
{
  SDL_Point fAcc = {0, 0};
  SDL_Point fVel = {0, 0};
  SDL_Point fPos;

  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;
  ESGE_Sound *jmpSnd;

  bool facingR;
public:
  static int GetPosX(void *obj);
  static void SetPosX(void *obj, int value);

  static int GetPosY(void *obj);
  static void SetPosY(void *obj, int value);

  ObjPlayer(void);
  virtual ~ObjPlayer(void) override;

  virtual void OnStart(void) override;

  virtual void OnUpdate(void) override;

  virtual void OnPhysic(void) override;

  virtual void OnCollide(ESGE_ObjCollider *other) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
#ifdef ESGE_EDITOR
  virtual void OnEditorInit(void) override;
  virtual void OnEditorQuit(void) override;
#endif
};

#endif