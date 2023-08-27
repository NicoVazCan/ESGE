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

    pos.y -= vel.y;

    thisColBox = ESGE_ObjCollider::GetBox();

    if (vel.x > 0)
    {
      if ((otherH = ESGE_ObjStatic::GetObjAtRight(thisColBox)))
      {
        otherH->OnCollideLeft(this);
        OnCollideRight(otherH);
      }
    }
    else if (vel.x < 0)
    {
      if ((otherH = ESGE_ObjStatic::GetObjAtLeft(thisColBox)))
      {
        otherH->OnCollideRight(this);
        OnCollideLeft(otherH);
      }
    }

    pos.y += vel.y;
  }

  if (vel.y != 0)
  {
    SDL_Rect thisColBox;

    thisColBox = ESGE_ObjCollider::GetBox();

    if (vel.y > 0)
    {
      if((otherV = ESGE_ObjStatic::GetObjAtDown(thisColBox)))
      {
        otherV->OnCollideUp(this);
        OnCollideDown(otherV);
      }
    }
    else if (vel.y < 0)
    {
      if((otherV = ESGE_ObjStatic::GetObjAtUp(thisColBox)))
      {
        otherV->OnCollideDown(this);
        OnCollideUp(otherV);
      }
    }
  }

  prevPos = pos;
}
