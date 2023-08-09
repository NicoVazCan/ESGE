#include "ESGE_objDynamic.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_objStatic.h"

ESGE_ObjDynamic *ESGE_ObjDynamic::list = NULL;


ESGE_ObjDynamic::ESGE_ObjDynamic(void)
{}

ESGE_ObjDynamic::~ESGE_ObjDynamic(void)
{}


void
ESGE_ObjDynamic::EnableDynamic(void)
{
  enabledDynamic = true;
  EnablePhysic();
  SGLIB_LIST_ADD(
    ESGE_ObjDynamic,
    list,
    this,
    next
  );
}

void
ESGE_ObjDynamic::DisableDynamic(void)
{
  enabledDynamic = false;
  DisablePhysic();
  SGLIB_LIST_DELETE(
    ESGE_ObjDynamic,
    list,
    this,
    next
  );
}

bool
ESGE_ObjDynamic::IsEnabledDynamic(void)
{
  return enabledDynamic;
}


void
ESGE_ObjDynamic::OnPhysic(void)
{
  SDL_Rect thisColBox;

  thisColBox = GetColBox();

  if (prevPos.x != pos.x || prevPos.y != pos.y)
  {
    SDL_Point vel;

    vel.x = pos.x - prevPos.x;
    vel.y = pos.y - prevPos.y;

    thisColBox.y -= vel.y;

    if (vel.x > 0)
    {
      SDL_Point test;
      ESGE_ObjStatic *other;

      test.x = thisColBox.x + thisColBox.w - 1;
      test.y = thisColBox.y;

      while (test.y < thisColBox.y + thisColBox.h)
      {
        if ((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.x = otherColBox.x - thisColBox.w;

          OnCollide(other);
          other->OnCollide(this);
          break;
        }
        test.y += ESGE_OBJ_STATIC_H;
      }
      if (test.y >= thisColBox.y + thisColBox.h)
      {
        test.y--;

        if ((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.x = otherColBox.x - thisColBox.w;

          OnCollide(other);
          other->OnCollide(this);
        }
      }
    }
    else if (vel.x < 0)
    {
      SDL_Point test;
      ESGE_ObjStatic *other;

      test.x = thisColBox.x;
      test.y = thisColBox.y;

      while (test.y < thisColBox.y + thisColBox.h)
      {
        if ((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.x = otherColBox.x + otherColBox.w;

          OnCollide(other);
          other->OnCollide(this);
          break;
        }
        test.y += ESGE_OBJ_STATIC_H;
      }
      if (test.y >= thisColBox.y + thisColBox.h)
      {
        test.y--;

        if ((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.x = otherColBox.x + otherColBox.w;

          OnCollide(other);
          other->OnCollide(this);
        }
      }
    }

    thisColBox.y += vel.y;

    if (vel.y > 0)
    {
      SDL_Point test;
      ESGE_ObjStatic *other;

      test.x = thisColBox.x;
      test.y = thisColBox.y + thisColBox.h - 1;

      while (test.x < thisColBox.x + thisColBox.w)
      {
        if((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.y = otherColBox.y - thisColBox.h;

          OnCollide(other);
          other->OnCollide(this);
          break;
        }
        test.x += ESGE_OBJ_STATIC_W;
      }
      if (test.x >= thisColBox.x + thisColBox.w)
      {
        test.x--;

        if ((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.y = otherColBox.y - thisColBox.h;

          OnCollide(other);
        }
      }
    }
    else if (vel.y < 0)
    {
      SDL_Point test;
      ESGE_ObjStatic *other;

      test.x = thisColBox.x;
      test.y = thisColBox.y;

      while (test.x < thisColBox.x + thisColBox.w)
      {
        if((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.y = otherColBox.y + otherColBox.h;

          OnCollide(other);
          other->OnCollide(this);
          break;
        }
        test.x += ESGE_OBJ_STATIC_W;
      }
      if (test.x >= thisColBox.x + thisColBox.w)
      {
        test.x--;

        if ((other = ESGE_ObjStatic::GetObjAt(test)))
        {
          SDL_Rect otherColBox;

          otherColBox = other->GetColBox();
          thisColBox.y = otherColBox.y + otherColBox.h;

          OnCollide(other);
          other->OnCollide(this);
        }
      }
    }
    SetColBox(thisColBox);
    prevPos = pos;
  }

  for (
    ESGE_ObjDynamic *other = this->next;
    other != NULL;
    other = other->next
  )
  {
    SDL_Rect otherColBox;

    otherColBox = other->GetColBox();

    if (SDL_HasIntersection(&thisColBox, &otherColBox))
    {
      OnCollide(other);
      other->OnCollide(this);
    }
  }
}
