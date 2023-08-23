#ifndef ENEMY_H_
# define ENEMY_H_

#include <SDL2/SDL.h>
#include "alive.h"


class ObjEnemy: public ObjAlive
{
  static ObjEnemy *list;

  ObjEnemy *next;
  bool enabledEnemy = false;

public:
  static void Attack(SDL_Rect atkBox, int dmg);

  ObjEnemy(void);
  virtual ~ObjEnemy(void) = 0;
  
  void EnableEnemy(void);
  void DisableEnemy(void);
  bool IsEnabledEnemy(void);
};

#endif