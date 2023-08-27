#include "ESGE_objDynamic.h"

#include "ESGE_objStatic.h"


ESGE_ObjDynamic::ESGE_ObjDynamic(void)
{}

ESGE_ObjDynamic::~ESGE_ObjDynamic(void)
{}


void
ESGE_ObjDynamic::OnPhysic(void)
{
  SDL_Point vel;
  ESGE_ObjStatic *otherH = NULL, *otherV = NULL;

  vel.x = pos.x - prevPos.x;
  vel.y = pos.y - prevPos.y;

  if (vel.x != 0)
  {
    SDL_Rect thisColBox;

    thisColBox = GetColBox();

    thisColBox.y -= vel.y;

    if (vel.x > 0)
    {
      if ((otherH = ESGE_ObjStatic::GetObjAtRight(thisColBox)))
      {
        SDL_Rect otherColBox;

        otherColBox = otherH->GetColBox();
        thisColBox.x = otherColBox.x - thisColBox.w;
      }
    }
    else if (vel.x < 0)
    {
      if ((otherH = ESGE_ObjStatic::GetObjAtLeft(thisColBox)))
      {
        SDL_Rect otherColBox;

        otherColBox = otherH->GetColBox();
        thisColBox.x = otherColBox.x + otherColBox.w;
      }
    }

    thisColBox.y += vel.y;

    SetColBox(thisColBox);
  }

  if (vel.y != 0)
  {
    SDL_Rect thisColBox;

    thisColBox = GetColBox();

    if (vel.y > 0)
    {
      if((otherV = ESGE_ObjStatic::GetObjAtDown(thisColBox)))
      {
        SDL_Rect otherColBox;

        otherColBox = otherV->GetColBox();
        thisColBox.y = otherColBox.y - thisColBox.h;
      }
    }
    else if (vel.y < 0)
    {
      if((otherV = ESGE_ObjStatic::GetObjAtUp(thisColBox)))
      {
        SDL_Rect otherColBox;

        otherColBox = otherV->GetColBox();
        thisColBox.y = otherColBox.y + otherColBox.h;
      }
    }

    SetColBox(thisColBox);
  }

  if (otherH)
  {
    OnCollide(otherH);
    otherH->OnCollide(this);
  }
  if (otherV)
  {
    OnCollide(otherV);
    otherV->OnCollide(this);
  }

  prevPos = pos;
}
