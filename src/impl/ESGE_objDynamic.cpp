#include "ESGE_objDynamic.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

ESGE_ObjDynamic *ESGE_ObjDynamic::list = NULL;

ESGE_ObjDynamic::ESGE_ObjDynamic(
  SDL_Point pos,
  SDL_Point vel,
  SDL_Point acc,
  SDL_Rect offsetSize
):
  ESGE_ObjPoint(pos),
  ESGE_ObjMove(pos, vel, acc),
  ESGE_ObjCollider(pos, offsetSize)
{
  SGLIB_LIST_ADD(
    ESGE_ObjDynamic,
    list,
    this,
    next
  );
}

ESGE_ObjDynamic::~ESGE_ObjDynamic(void)
{
  SGLIB_LIST_DELETE(
    ESGE_ObjDynamic,
    list,
    this,
    next
  );
}

void
ESGE_ObjDynamic::OnMove(void)
{
  SDL_Rect thisColBox, otherColBox;

  thisColBox = GetColBox();

  vel.x += acc.x;
  vel.y += acc.y;

  thisColBox.x += vel.x;

  if (vel.x > 0)
  {
    for (
      SDL_Point test = {
        thisColBox.x + thisColBox.w - 1,
        thisColBox.y
      };
      test.y <= thisColBox.y + thisColBox.h;
      test.y += ESGE_OBJ_STATIC_V_RES
    )
    {
      for (
        ESGE_ObjStatic *other = ESGE_ObjStatic::GetObjsAt(test);
        other != NULL;
        other = other->next
      )
      {
        otherColBox = other->GetColBox();

        if (SDL_HasIntersection(&thisColBox, &otherColBox))
        {
          OnCollide(other);
          other->OnCollide(this);

          thisColBox.x = otherColBox.x - thisColBox.w;

          vel.x = 0;
          acc.x = 0;
        }
      }
    }
  }
  else if (vel.x < 0)
  {
    for (
      SDL_Point test = {thisColBox.x, thisColBox.y};
      test.y <= thisColBox.y + thisColBox.h;
      test.y += ESGE_OBJ_STATIC_V_RES
    )
    {
      for (
        ESGE_ObjStatic *other = ESGE_ObjStatic::GetObjsAt(test);
        other != NULL;
        other = other->next
      )
      {
        otherColBox = other->GetColBox();

        if (SDL_HasIntersection(&thisColBox, &otherColBox))
        {
          OnCollide(other);
          other->OnCollide(this);

          thisColBox.x = otherColBox.x + otherColBox.w;

          vel.x = 0;
          acc.x = 0;
        }
      }
    }
  }

  thisColBox.y += vel.y;

  if (vel.y > 0)
  {
    for (
      SDL_Point test = {
        thisColBox.x,
        thisColBox.y + thisColBox.h - 1
      };
      test.x <= thisColBox.x + thisColBox.w;
      test.x += ESGE_OBJ_STATIC_H_RES
    )
    {
      for (
        ESGE_ObjStatic *other = ESGE_ObjStatic::GetObjsAt(test);
        other != NULL;
        other = other->next
      )
      {
        otherColBox = other->GetColBox();

        if (SDL_HasIntersection(&thisColBox, &otherColBox))
        {
          OnCollide(other);
          other->OnCollide(this);

          thisColBox.y = otherColBox.y - thisColBox.h;
        }
      }
    }
  }
  else if (vel.y < 0)
  {
    for (
      SDL_Point test = {thisColBox.x, thisColBox.y};
      test.x <= thisColBox.x + thisColBox.w;
      test.x += ESGE_OBJ_STATIC_H_RES
    )
    {
      for (
        ESGE_ObjStatic *other = ESGE_ObjStatic::GetObjsAt(test);
        other != NULL;
        other = other->next
      )
      {
        otherColBox = other->GetColBox();

        if (SDL_HasIntersection(&thisColBox, &otherColBox))
        {
          OnCollide(other);
          other->OnCollide(this);

          thisColBox.y = otherColBox.y + otherColBox.h;
        }
      }
    }
  }

  for (
    ESGE_ObjDynamic *other = this->next;
    other != NULL;
    other = other->next
  )
  {
    otherColBox = other->GetColBox();

    if (SDL_HasIntersection(&thisColBox, &otherColBox))
    {
      OnCollide(other);
      other->OnCollide(this);
    }
  }

  SetColBox(thisColBox);
}
