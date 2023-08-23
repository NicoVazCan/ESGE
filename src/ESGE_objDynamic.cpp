#include "ESGE_objDynamic.h"

#include "ESGE_objStatic.h"


ESGE_ObjDynamic::ESGE_ObjDynamic(void)
{}

ESGE_ObjDynamic::~ESGE_ObjDynamic(void)
{}


void
ESGE_ObjDynamic::OnPhysic(void)
{
  SDL_Rect thisColBox;

  thisColBox = GetColBox();

  if (prevPos.x != pos.x || prevPos.y != pos.y)
  {
    SDL_Point vel;
    ESGE_ObjStatic *otherH = NULL, *otherV = NULL;

    vel.x = pos.x - prevPos.x;
    vel.y = pos.y - prevPos.y;

    thisColBox.y -= vel.y;

    if (vel.x > 0)
    {
      SDL_Point test;

      test.x = thisColBox.x + thisColBox.w - 1;
      test.y = thisColBox.y;

      while (test.y < thisColBox.y + thisColBox.h)
      {
        if ((otherH = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherH->GetColBox();
          thisColBox.x = otherColBox.x - thisColBox.w;
          break;
        }
        test.y += ESGE_ObjStatic::cellH;
      }
      if (!otherH)
      {
        test.y = thisColBox.y + thisColBox.h - 1;

        if ((otherH = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherH->GetColBox();
          thisColBox.x = otherColBox.x - thisColBox.w;
        }
      }
    }
    else if (vel.x < 0)
    {
      SDL_Point test;

      test.x = thisColBox.x;
      test.y = thisColBox.y;

      while (test.y < thisColBox.y + thisColBox.h)
      {
        if ((otherH = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherH->GetColBox();
          thisColBox.x = otherColBox.x + otherColBox.w;
          break;
        }
        test.y += ESGE_ObjStatic::cellH;
      }
      if (!otherH)
      {
        test.y = thisColBox.y + thisColBox.h - 1;

        if ((otherH = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherH->GetColBox();
          thisColBox.x = otherColBox.x + otherColBox.w;
        }
      }
    }

    thisColBox.y += vel.y;

    if (vel.y > 0)
    {
      SDL_Point test;

      test.x = thisColBox.x;
      test.y = thisColBox.y + thisColBox.h - 1;

      while (test.x < thisColBox.x + thisColBox.w)
      {
        if((otherV = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherV->GetColBox();
          thisColBox.y = otherColBox.y - thisColBox.h;
          break;
        }
        test.x += ESGE_ObjStatic::cellW;
      }
      if (!otherV)
      {
        test.x = thisColBox.x + thisColBox.w - 1;

        if ((otherV = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherV->GetColBox();
          thisColBox.y = otherColBox.y - thisColBox.h;
        }
      }
    }
    else if (vel.y < 0)
    {
      SDL_Point test;

      test.x = thisColBox.x;
      test.y = thisColBox.y;

      while (test.x < thisColBox.x + thisColBox.w)
      {
        if((otherV = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherV->GetColBox();
          thisColBox.y = otherColBox.y + otherColBox.h;
          break;
        }
        test.x += ESGE_ObjStatic::cellW;
      }
      if (!otherV)
      {
        test.x = thisColBox.x + thisColBox.w - 1;

        if ((otherV = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = otherV->GetColBox();
          thisColBox.y = otherColBox.y + otherColBox.h;          
        }
      }
    }

    SetColBox(thisColBox);
      

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
}
