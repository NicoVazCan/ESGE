#include "ESGE_objCollider.h"

ESGE_ObjCollider::ESGE_ObjCollider(void)
{}

ESGE_ObjCollider::~ESGE_ObjCollider(void)
{}


SDL_Rect
ESGE_ObjCollider::GetColBox(void)
{
  SDL_Rect colBox = offsetSize;

  colBox.x += pos.x;
  colBox.y += pos.y;

  return colBox;
}

void
ESGE_ObjCollider::SetColBox(SDL_Rect colBox)
{
  pos.x = colBox.x - offsetSize.x;
  pos.y = colBox.y - offsetSize.y;
}

void
ESGE_ObjCollider::OnCollide(SDL_UNUSED ESGE_ObjCollider *other)
{}
