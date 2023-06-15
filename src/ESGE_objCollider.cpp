#include "ESGE_objCollider.h"

ESGE_ObjCollider::ESGE_ObjCollider(SDL_Point pos, SDL_Rect colBox):
  ESGE_ObjPoint(pos),
  colBox(colBox)
{}

ESGE_ObjCollider::~ESGE_ObjCollider(void) {}

SDL_Rect
ESGE_ObjCollider::GetColBox(void)
{
  SDL_Rect worldColBox = colBox;

  worldColBox.x += pos.x;
  worldColBox.y += pos.x;

  return worldColBox;
}