#include "enemy.h"


ObjEnemy *ObjEnemy::list = NULL;


void
ObjEnemy::Attack(SDL_Rect atkBox, int dmg)
{
  for (ObjEnemy *obj = list; obj != NULL; obj = obj->next)
  {
    SDL_Rect hitBox;

    hitBox = obj->GetHitBox();
    if (SDL_HasIntersection(&atkBox, &hitBox))
    {
      obj->OnAttack(dmg);
    }
  }
}


ObjEnemy::ObjEnemy(void)
{}

ObjEnemy::~ObjEnemy(void)
{}


void
ObjEnemy::EnableEnemy(void)
{
  enabledEnemy = true;

  next = list;
  list = this;
}

void
ObjEnemy::DisableEnemy(void)
{
  ObjEnemy **node;

  enabledEnemy = false;

  for (node = &list; *node && *node != this; node = &(*node)->next);
  SDL_assert(*node && *node == this);
  *node = this->next;
}

bool
ObjEnemy::IsEnabledEnemy(void)
{
  return enabledEnemy;
}