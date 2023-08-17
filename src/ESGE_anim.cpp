#include "ESGE_anim.h"

void
ESGE_AnimPlayer::Start(const ESGE_Anim *anim)
{
  SDL_assert(anim);

  this->anim = anim;
  frm        = 0;
  ticks      = 0;
  rep        = anim->rep;
}

void
ESGE_AnimPlayer::Update(Uint32 deltaTm)
{
  if (rep < 0 || rep > 0 || !done)
  {
    Uint32 dur = anim->frms[frm].dur * 100;

    while (ticks >= dur)
    {
      ticks -= dur;

      frm++;

      if (frm == anim->nFrms)
      {
        frm = 0;
        if (rep == 0) done = true;
        else          rep--;
      }
    }
    if (speed != 0)
    {
      Uint32 deltaTicks;

      deltaTicks = speed * deltaTm;

      if (
        deltaTm > SDL_MAX_UINT32 / speed ||
        ticks > SDL_MAX_UINT32 - deltaTicks
      )
        ticks = SDL_MAX_UINT32;
      else
        ticks += deltaTicks;
    }
  }
}

bool
ESGE_AnimPlayer::IsDone(void)
{
  return done;
}


void
ESGE_AnimPlayer::GetSprite(ESGE_Sprite *sprt)
{
  const ESGE_Frm *pFrm;

  SDL_assert(sprt);

  pFrm = anim->frms + frm;

  sprts->GetSprite(sprt, pFrm->col, pFrm->row);

  sprt->scale  = pFrm->scale;
  sprt->angle  = pFrm->angle;
  sprt->center = pFrm->center;
  sprt->flip   = pFrm->flip;
}
