#ifndef ESGE_ANIM_H_
# define ESGE_ANIM_H_

# include <SDL2/SDL.h>
# include "ESGE_display.h"
# include "ESGE_spritesheet.h"


struct ESGE_Frm
{
  int col, row;
  double scale, angle;
  SDL_Point center;
  SDL_RendererFlip flip;
  Uint32 dur;
};

struct ESGE_Anim
{
  const ESGE_Frm *frms;
  size_t nFrms;
  int rep;
};

struct ESGE_AnimPlayer
{
  ESGE_Spritesheet *sprts;
  const ESGE_Anim *anim;
  size_t frm;
  Uint32 ticks, speed;
  int rep;
  bool done;

  void Start(const ESGE_Anim *anim);
  void Update(Uint32 deltaTm);
  bool IsDone(void);

  void GetSprite(ESGE_Sprite *sprt);
};

#endif