#include "alive.h"

ObjAlive::ObjAlive(void)
{}

ObjAlive::~ObjAlive(void)
{}


SDL_Rect
ObjAlive::GetHitBox(void)
{
  SDL_Rect hitBox = offsetSize;

  hitBox.x += pos.x;
  hitBox.y += pos.y;

  return hitBox;
}

void
ObjAlive::SetHitBox(SDL_Rect hitBox)
{
  pos.x = hitBox.x - offsetSize.x;
  pos.y = hitBox.y - offsetSize.y;
}


void
ObjAlive::OnAttack(SDL_UNUSED int dmg)
{}
