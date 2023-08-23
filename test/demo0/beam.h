#ifndef BEAM_H_
# define BEAM_H_

#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_objUpdate.h"
#include "ESGE_anim.h"

#include "ESGE_objDrawSprite.h"

class ObjBeam:
  public ESGE_ObjScene,
  public ESGE_ObjUpdate,
  public ESGE_ObjDrawSprite
{
  enum {R, L, D, U} dir = R;
  SDL_Point fAcc = {0, 0};
  SDL_Point fVel = {0, 0};
  SDL_Point fPos;

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

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
#ifdef ESGE_EDITOR
  virtual void OnEditorInit(void) override;
  virtual void OnEditorQuit(void) override;
#endif
};

#endif