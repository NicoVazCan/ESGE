#ifndef BEAM_H_
# define BEAM_H_

#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_anim.h"

#include "ESGE_objUpdate.h"
#include "ESGE_objDynamic.h"
#include "ESGE_objDrawSprite.h"

enum BeamDir {BEAM_R, BEAM_L, BEAM_D, BEAM_U};

class ObjBeam:
  public ESGE_ObjScene,
  public ESGE_ObjUpdate,
  public ESGE_ObjDynamic,
  public ESGE_ObjDrawSprite
{
  BeamDir dir = BEAM_R;
  SDL_Point fAcc = {0, 0};
  SDL_Point fVel = {0, 0};
  SDL_Point fPos;
  SDL_Point iPos = {0, 0};

  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;

public:
  static int GetPosX(void *obj);
  static void SetPosX(void *obj, int value);

  static int GetPosY(void *obj);
  static void SetPosY(void *obj, int value);

  static int GetDir(void *obj);
  static void SetDir(void *obj, int value);

  ObjBeam(void);
  virtual ~ObjBeam(void) override;

  virtual void OnUpdate(void) override;

  virtual void OnCollideUp(ESGE_ObjCollider *other) override;
  virtual void OnCollideDown(ESGE_ObjCollider *other) override;
  virtual void OnCollideLeft(ESGE_ObjCollider *other) override;
  virtual void OnCollideRight(ESGE_ObjCollider *other) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
#ifdef ESGE_EDITOR
  virtual void OnEditorEnable(void) override;
  virtual void OnEditorDisable(void) override;
#endif
};

#endif